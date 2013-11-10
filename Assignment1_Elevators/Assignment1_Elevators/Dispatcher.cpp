/*****************************************************
 * EECE 314 Assignment #1
 * Date: October 2013
 * Authors: 
 *	 Kieren Wou	54724117
 *	 Ryan Wong	47307103
 *****************************************************/
#include "Dispatcher.h"
#include <memory>
#include <sstream>
#include <vector>

Dispatcher::Dispatcher()
{}

Dispatcher::Dispatcher(IOProgramPtr_t pIoProgram, int numberOfElevators)
	: m_pIoProgram(pIoProgram)  // have a pointer to the IOProgram object so that the dispatcher can post to its mailbox
	, m_bExit(false)
	, m_bIsStartUp(true)
	, m_numberOfElevators(numberOfElevators)
{
	m_getCommandFromIO = new CMutex("GetCommandFromIO");;
	m_screenMutex = new CMutex("PrintToScreen");
	std::stringstream ss;
	std::string elevatorNumber;
	for(int i = 1; i <= m_numberOfElevators; ++i)
	{
		ss << i;
		elevatorNumber = ss.str();
		m_pElevatorDataPool[i-1] = new CDataPool("Elevator"+elevatorNumber+"Status",sizeof(ElevatorStatus_t)); 
		m_pElevatorCommands[i-1] = new CPipe("Elevator"+elevatorNumber+"Commands",1024);
		ss.str("");
	}

	for(int i = 0; i < m_numberOfElevators; ++i)
	{
		//link to datapools of elevator statuses
		m_pElevatorStatus[i] = (ElevatorStatusPtr_t)(m_pElevatorDataPool[i]->LinkDataPool());
	}
	
	m_pEntryToQueue = new CSemaphore("EntryToQueue", 0 , m_numberOfElevators); // entry
	m_pExitFromQueue = new CSemaphore("ExitFromQueue", 0, m_numberOfElevators); // exit
	m_pQueueEmpty = new CSemaphore("QueueEmpty", 0, m_numberOfElevators); // empty
	m_pQueueFull = new CSemaphore("QueueFull", 0, m_numberOfElevators); // full

	m_pQueueMutex = new CMutex("QueueMutex");
}

Dispatcher::~Dispatcher()
{
	for(int i = 0; i < m_numberOfElevators; ++i)
	{
		delete m_pElevatorDataPool[i];
		delete m_pElevatorCommands[i];
	}
	
	delete m_getCommandFromIO;
	delete m_screenMutex;

	delete m_pEntryToQueue;
	delete m_pExitFromQueue;
	delete m_pQueueEmpty;
	delete m_pQueueFull;
}

//Thread that receives the updated information from the elevator data pools
int Dispatcher::CollectElevatorStatus(void* args)
{
	std::stringstream ss;
	int elevatorId = *(int*)args;
	ss << elevatorId;
	std::string elevatorNumber = ss.str();

	CSemaphore dispatcherToElevator_consumer("DispatcherToElevator"+elevatorNumber+"Consumer",1,1);
	CSemaphore dispatcherToElevator_producer("DispatcherToElevator"+elevatorNumber+"Producer",0,1);

	do{
		if(dispatcherToElevator_producer.Read() > 0) 
		{
			if(elevatorId-1 >= 0)
			{
				dispatcherToElevator_producer.Wait();

				m_pEntryToQueue->Wait();
				m_pQueueFull->Signal();
				
				m_localElevatorStatus[elevatorId-1] = *m_pElevatorStatus[elevatorId-1];
				OutputDebugString("Dispatcher Child reading from Elevator DP and copying to Dispatcher Main. Waiting at 'exit roller coaster' semaphores\n");
				
				m_pExitFromQueue->Wait();
				m_pQueueEmpty->Signal();
				
				dispatcherToElevator_consumer.Signal();
			}
		}
	} while(!m_bExit);
	return 0;
}

// Thread to read pipeline with commands from IOProgram
int Dispatcher::ReadFromIoToDispatcherPipeline(void *args)
{ 
	CPipe IoToDispatcher_pipeline(k_ioToDispatcherPipeline, 1024);
	CSemaphore dispatcherUserInput_consumer("DispatcherUserInputConsumer",1,1);
	CSemaphore dispatcherUserInput_producer("DispatcherUserInputProducer",0,1);

	UserInputData_t userInput;
	do{

		if(IoToDispatcher_pipeline.TestForData() >= sizeof(UserInputData_t))
		{
			m_bIsStartUp = false;
			IoToDispatcher_pipeline.Read(&userInput, sizeof(UserInputData_t));	//Read commands from IO 
			OutputDebugString("Dispatcher reading UserInputData_t from IO\n");
		
			// "EE" used to terminate simulation
			if(userInput.direction == 'E' && userInput.floor == 'E') 
			{ 
				BOOL bPostSuccessful = m_pIoProgram->Post(k_terminateSimulation);
				printf("Posting Message. Status: %d\n", bPostSuccessful); 
				if(bPostSuccessful)
				{
					m_bExit = true;
					break;
				}
			}
			FloorRequest_t floorReq;
			int direction, floorNumber, elevId;
			bool bInsideRequest;
			if(userInput.direction != 'U' && userInput.direction != 'D') // inside request
			{
				elevId = userInput.direction - '0';
				floorNumber = userInput.floor - '0';

				FloorRequest_t insideFR(floorNumber, INT_MAX, elevId);
				floorReq = insideFR;
			}
			else // outside request
			{
				assert(userInput.direction == 'U' || userInput.direction == 'D');
				if(userInput.direction == 'U')
					direction = k_directionUp;

				else if(userInput.direction == 'D')
					direction = k_directionDown;
				
				assert(userInput.floor >= '0' && userInput.floor <= '9');
				if(userInput.floor >= '0' && userInput.floor <= '9')
					floorNumber = userInput.floor - '0';

				FloorRequest_t outsideFR(floorNumber, direction);
				floorReq = outsideFR;
			}

			m_pQueueMutex->Wait();
			m_floorRequestVect.push_back(floorReq);
			m_pQueueMutex->Signal();
			OutputDebugString("Dispatcher Child adding FR to queue\n");
		}
	} while(1);

	return 0;
}

int Dispatcher::main()
{
	std::vector<ClassThread<Dispatcher>*> collectElevatorStatusVect;
	std::vector<ClassThread<Dispatcher>*> dispatcherToElevatorVect;
	std::vector<CSemaphore*> dispatcherLocalElevatorStatusConsumerVect;
	std::vector<CSemaphore*> dispatcherLocalElevatorStatusProducerVect;

	int xArray[100];

	CSemaphore floorRequestVectProtector_consumer("FloorRequestVectProtectorConsumer",1,1);  // semaphores to protect floor requests
	CSemaphore floorRequestVectProtector_producer("FloorRequestVectProtectorProducer",0,1);

	std::stringstream ss;
	std::string elevatorNumberStr;
	bool bIsStartUp = true;
	ClassThread<Dispatcher>	 IoToDispatcherPipeline(this,&Dispatcher::ReadFromIoToDispatcherPipeline,ACTIVE, NULL);
	
	for( int i = 1; i <= m_numberOfElevators; i++)
	{
		ss << i;
		elevatorNumberStr = ss.str();
		xArray[i-1] = i;

		if(i - 1 >= 0)
		{	
			ClassThread<Dispatcher>* pCollectElevatorStatus= new ClassThread<Dispatcher>(this,&Dispatcher::CollectElevatorStatus, ACTIVE, &xArray[i-1]);
			collectElevatorStatusVect.push_back(pCollectElevatorStatus);
		}
	}
	
	FloorRequestVect_t floorRequestQueue;
	ElevatorStatusVect_t elevatorStatusVect;

	do{
		elevatorStatusVect.clear();

		for(int i = 0; i < m_numberOfElevators; ++i)
		{
			m_pEntryToQueue->Signal();
		}
		for(int i = 0; i < m_numberOfElevators; )
		{
			if(m_bExit)
			{
				OutputDebugString("breaking from dispatcher loop\n");
				break; 
			}
			if(m_pQueueFull->Wait(200) == WAIT_TIMEOUT)
				continue;
			++i;
		}
		
		// obtain Elevator Statuses here
		for(int i = 0; i < m_numberOfElevators; ++i)
		{
			elevatorStatusVect.push_back(m_localElevatorStatus[i]);
		}
		
		OutputDebugString("Dispatcher Main has read each of the Elevator Statuses\n");

		// release semaphores
		for(int i = 0; i < m_numberOfElevators; ++i)
		{
			m_pExitFromQueue->Signal();
		}
		for(int i = 0; i < m_numberOfElevators;)
		{
			if(m_bExit)
			{
				OutputDebugString("breaking from dispatcher loop\n");
				break; 
			}
			if(m_pQueueEmpty->Wait(200) == WAIT_TIMEOUT)
				continue;
			++i;
		}

		FloorRequest_t tempFloorRequest;
		FloorRequestVect_t outputDispatcher;
		
		m_pQueueMutex->Wait();
		outputDispatcher = FSAlgorithm::DispatcherFsCalculator(elevatorStatusVect, m_floorRequestVect, m_bIsStartUp);
		m_pQueueMutex->Signal();

		for(auto iter = outputDispatcher.begin(); iter != outputDispatcher.end(); ++iter)
		{
			int elevatorId = std::distance(outputDispatcher.begin(), iter);
			tempFloorRequest = *iter;
			m_pElevatorCommands[elevatorId]->Write(&tempFloorRequest, sizeof(FloorRequest_t)); // send FR to elevator
			OutputDebugString("Dispatcher Main has sent FR to each Elevator Pipeline\n");
		}

		if(m_bExit)
		{
			OutputDebugString("breaking from dispatcher loop\n");
			break; 
		}

	} while(1);

	IoToDispatcherPipeline.WaitForThread();
	
	for( int i = 0; i < m_numberOfElevators; i++)
	{
		delete collectElevatorStatusVect[i];
	}
	
	OutputDebugString("Exiting dispatcher\n");
	return 0;
}