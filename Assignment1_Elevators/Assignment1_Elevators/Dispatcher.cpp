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
	, m_bExit(FALSE)
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
//		printf("Created %d datapools in IO Program\n", i);
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


//Thread to write commands to Elevators
int Dispatcher::DispatcherToElevator(void* args)
{
	std::stringstream ss;
	int elevatorId = *(int*)args;
	ss << elevatorId;
	std::string elevatorNumber = ss.str();
	CPipe elevatorCommands("Elevator"+elevatorNumber+"Commands", 1024);
	do{
		
//		printf("%c\n",elevatorNumber);
	} while(1);
	return 0;
}

//testing to see if dispatcher is getting the right information from the data pools
void Dispatcher::UpdateElevatorStatus(ElevatorStatus_t elevatorStatus, int elevatorNumber) const
{
	m_screenMutex->Wait();	
		MOVE_CURSOR(25,elevatorNumber*5+5);

	printf("Elevator %d from Dispatcher\n", elevatorNumber);
	MOVE_CURSOR(25,elevatorNumber*5+6);
	printf("Direction: %d\n",elevatorStatus.direction);
	MOVE_CURSOR(25,elevatorNumber*5+7);
	printf("Door Status: %d\n",elevatorStatus.doorStatus);
	MOVE_CURSOR(25,elevatorNumber*5+8);
	printf("Floor Number: %d\n", elevatorStatus.floorNumber);
	m_screenMutex->Signal();
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
				OutputDebugString("Dispatcher Child reading from Elevator DP and copying to Dispatcher Main\n");
				dispatcherToElevator_producer.Wait();
				OutputDebugString("dispatcherToElevator_producer.Wait() finished call. Waiting at 'enter roller coaster' semaphores.\n");
				m_pEntryToQueue->Wait();
				m_pQueueFull->Signal();
				
				m_localElevatorStatus[elevatorId-1] = *m_pElevatorStatus[elevatorId-1];
				OutputDebugString("Dispatcher Child reading from Elevator DP and copying to Dispatcher Main. Waiting at 'exit roller coaster' semaphores\n");
				m_pExitFromQueue->Wait();
				m_pQueueEmpty->Signal();
				
				dispatcherToElevator_consumer.Signal();
				OutputDebugString("dispatcherToElevator_consumer.Signal() finished call\n");
				OutputDebugString("Dispatcher Child has read from Elevator DP and copied to Dispatcher Main\n");
//				UpdateElevatorStatus(m_localElevatorStatus[elevatorId-1],elevatorId);	//update visual for elevator 1
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
	CSemaphore floorRequestVectProtector_consumer("FloorRequestVectProtectorConsumer",1,1);  // semaphores to protect floor requests
	CSemaphore floorRequestVectProtector_producer("FloorRequestVectProtectorProducer",0,1);

	UserInputData_t userInput;
	do{

		if(IoToDispatcher_pipeline.TestForData() >= sizeof(UserInputData_t))
		{
			IoToDispatcher_pipeline.Read(&userInput, sizeof(UserInputData_t));	//Read commands from IO 
			OutputDebugString("Dispatcher reading UserInputData_t from IO\n");
		
// 			m_screenMutex->Wait(); 
// 			MOVE_CURSOR(0,1);
// 			printf("Direction = %c and Floor = %c from Dispatcher\n", userInput.direction, userInput.floor);
// 			m_screenMutex->Signal();

			// FIXME how are we dealing with INSIDE requests? we temporarily have a boolean "bInsideRequest"
//			FloorRequest_t floorReq;
		
			// "EE" used to terminate simulation
			if(userInput.direction == 'E' && userInput.floor == 'E') 
			{ 
				BOOL bPostSuccessful = m_pIoProgram->Post(k_terminateSimulation);
				printf("Posting Message. Status: %d\n", bPostSuccessful); 
				if(bPostSuccessful)
				{
					m_bExit = TRUE;
					break;
				}
			}

			int direction, floorNumber;

			if(userInput.direction >= '0' && userInput.direction <= '9') // inside request
			{
				bool bInsideRequest = true;
				continue; // FIXME temporary! not dealing with inside reqs right now
			}
			else // outside request
			{
				if(userInput.direction == 'U')
				{
					direction = k_directionUp;
				}
				else if(userInput.direction == 'D')
				{
					direction = k_directionDown;
				} 
				else
				{
					assert(!"invalid direction from IO");
				}

				if(userInput.floor >= '0' && userInput.floor <= '9')
					floorNumber = userInput.floor - '0';
				else
					assert(!"invalid floor from IO");

			}

			FloorRequest_t floorReq(floorNumber, direction);

			floorRequestVectProtector_consumer.Wait();
			m_floorRequestVect.push_back(floorReq);
			floorRequestVectProtector_producer.Signal();
			OutputDebugString("Dispatcher Child adding FR to queue\n");
		}
	} while(1);
	return 0;
}

int Dispatcher::main()
{
	
	CPipe IoToDispatcher_pipeline("DispatcherCommands", 1024);
	std::vector<ClassThread<Dispatcher>*> collectElevatorStatusVect;
	std::vector<ClassThread<Dispatcher>*> dispatcherToElevatorVect;
	std::vector<CSemaphore*> dispatcherLocalElevatorStatusConsumerVect;
	std::vector<CSemaphore*> dispatcherLocalElevatorStatusProducerVect;

	int xArray[100];

	CSemaphore floorRequestVectProtector_consumer("FloorRequestVectProtectorConsumer",1,1);  // semaphores to protect floor requests
	CSemaphore floorRequestVectProtector_producer("FloorRequestVectProtectorProducer",0,1);

	std::stringstream ss;
	std::string elevatorNumberStr;
	

	int numberDirection;
	for( int i = 1; i <= m_numberOfElevators; i++)
	{
		ss << i;
		elevatorNumberStr = ss.str();
		xArray[i-1] = i;
		if(i - 1 >= 0) // safety check
		{	
			ClassThread<Dispatcher>* pDispatcherToElevator= new ClassThread<Dispatcher>(this,&Dispatcher::DispatcherToElevator, ACTIVE, &xArray[i-1]);
			dispatcherToElevatorVect.push_back(pDispatcherToElevator);
		}
	}// FIXME add delete in for the pointers in the vectors
	ClassThread<Dispatcher>	 IoToDispatcherPipeline(this,&Dispatcher::ReadFromIoToDispatcherPipeline,ACTIVE, NULL);

	
	for( int i = 1; i <= m_numberOfElevators; i++)
	{
		if( i - 1 >= 0)
		{	
			ClassThread<Dispatcher>* pCollectElevatorStatus= new ClassThread<Dispatcher>(this,&Dispatcher::CollectElevatorStatus, ACTIVE, &xArray[i-1]);
			collectElevatorStatusVect.push_back(pCollectElevatorStatus);
//			printf("Created %d threads in IOProgram\n", i);
//			Sleep(500);
		}// FIXME add delete in for the pointers in the vectors //add waitfor thread at the end
	}

	Sleep(1000);

	FloorRequestVect_t floorRequestQueue;
	ElevatorStatusVect_t elevatorStatusVect;
	do{

		if(floorRequestVectProtector_producer.Read() > 0) //Check to see if there is a new command
		{
			// read from the member variable storing the floor request queue; to be used for FS value calculations
			floorRequestVectProtector_producer.Wait();
			floorRequestQueue = m_floorRequestVect;
			floorRequestVectProtector_consumer.Signal();
			OutputDebugString("Dispatcher Main reading from queue\n");
			
			/** debugging **/
			int i = 1;
			for(auto itQueue = floorRequestQueue.begin(); itQueue != floorRequestQueue.end(); ++itQueue)
			{
				m_screenMutex->Wait(); 
				MOVE_CURSOR(0,i++);
				printf("\nRequest... floor: %d\tdirection: %d\n", itQueue->floorNumber, itQueue->direction);
				m_screenMutex->Signal();
			}
			/** end debugging **/
		}

		OutputDebugString("Dispatcher Main attempting to read Elevator Statuses\n");
		for(int i = 0; i < m_numberOfElevators; ++i)
		{
			m_pEntryToQueue->Signal();
		}
		for(int i = 0; i < m_numberOfElevators; ++i)
		{
			m_pQueueFull->Wait();
		}
		OutputDebugString("Dispatcher Main reading Elevator Statuses\n");
		
		// obtain Elevator Statuses here
		for(int i = 0; i < m_numberOfElevators; ++i)
		{
			elevatorStatusVect.push_back(m_localElevatorStatus[i]);
		}

		OutputDebugString("Dispatcher Main releasing semaphores protecting ElevatorStatusVect\n");
		// release semaphores
		for(int i = 0; i < m_numberOfElevators; ++i)
		{
			m_pExitFromQueue->Signal();
		}
		for(int i = 0; i < m_numberOfElevators; ++i)
		{
			m_pQueueEmpty->Wait();
		}
		OutputDebugString("Dispatcher Main has released semaphores protecting ElevatorStatusVect\n");

		FloorRequest_t tempFloorRequest;
		FloorRequestVect_t outputDispatcher;
		outputDispatcher = FSAlgorithm::DispatcherFsCalculator(elevatorStatusVect, floorRequestQueue);
		for(auto iter = outputDispatcher.begin(); iter != outputDispatcher.end(); ++iter)
		{
			int elevatorId = std::distance(outputDispatcher.begin(), iter);
			tempFloorRequest = *iter;
			m_pElevatorCommands[elevatorId]->Write(&tempFloorRequest,sizeof(FloorRequest_t)); // send FR to elevator
			OutputDebugString("Dispatcher Main sending FR to each Elevator Pipeline\n");
		}

		if(m_bExit)
		{
			printf("breaking from dispatcher loop\n");
			break; 
		}

	} while(1);

	IoToDispatcherPipeline.WaitForThread();
	
	for( int i = 0; i < m_numberOfElevators; i++)
	{
		delete dispatcherToElevatorVect[i];
		delete collectElevatorStatusVect[i];
	}
	
	printf("Exiting dispatcher\n");
	return 0;
}