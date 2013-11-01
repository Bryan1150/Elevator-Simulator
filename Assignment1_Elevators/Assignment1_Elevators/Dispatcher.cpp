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
		//printf("Created %d datapools in IO Program\n", i);
	}

	for(int i = 0; i < m_numberOfElevators; ++i)
	{
		/*ss << i;
		elevatorNumber = ss.str();*/
		m_pElevatorStatus[i] = (ElevatorStatusPtr_t)(m_pElevatorDataPool[i]->LinkDataPool());		//link to datapools of elevator statuses
		//printf("Created %d datapool link in IO Program\n", i);
	}
	
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
		
		//printf("%c\n",elevatorNumber);
	}while(1);
	return 0;
}

//Thread for testing to see if dispatcher is getting the right information from the data pools
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

void Dispatcher::ConvertUserInputToFloorRequest_(UserInputData_t const& userInput)
{
}

//Update the information from the elevator data pools
int Dispatcher::CollectElevatorStatus(void* args)
{
	std::stringstream ss;
	int elevatorId = *(int*)args;
	ss << elevatorId;
	std::string elevatorNumber = ss.str();

	CSemaphore dispatcherToElevator_consumer("DispatcherToElevator"+elevatorNumber+"Consumer",1,1);
	CSemaphore dispatcherToElevator_producer("DispatcherToElevator"+elevatorNumber+"Producer",0,1);
	CSemaphore dispatcherLocalElevatorStatus_consumer("DispatcherLocalElevatorStatus"+elevatorNumber+"Consumer",1,1);
	CSemaphore dispatcherLocalElevatorStatus_producer("DispatcherLocalElevatorStatus"+elevatorNumber+"Producer",0,1);
	do{
		if(dispatcherToElevator_producer.Read() > 0) // elevator 1 produced data
			{
			  if(elevatorId-1 >= 0)
			  {
				dispatcherToElevator_producer.Wait();
				//printf("Copying data from elevator1Status in IO program\n");

				//Protect m_localElevatorStatus information
				//dispatcherLocalElevatorStatus_consumer.Wait(); 
				m_localElevatorStatus[elevatorId-1].direction = m_pElevatorStatus[elevatorId-1]->direction;
				m_localElevatorStatus[elevatorId-1].doorStatus = m_pElevatorStatus[elevatorId-1]->doorStatus;
				m_localElevatorStatus[elevatorId-1].floorNumber = m_pElevatorStatus[elevatorId-1]->floorNumber;
				//dispatcherLocalElevatorStatus_producer.Signal();
				dispatcherToElevator_consumer.Signal();
				UpdateElevatorStatus(m_localElevatorStatus[elevatorId-1],elevatorId);	//update visual for elevator 1
			  }
			}
	}while(!m_bExit);
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

		
		IoToDispatcher_pipeline.Read(&userInput, sizeof(UserInputData_t));	//Read commands from IO 

		//m_screenMutex->Wait(); 
		//MOVE_CURSOR(0,1);
		//printf("Direction = %c and Floor = %c from Disptcher\n", userInput.direction, userInput.floor);
		//m_screenMutex->Signal();

		FloorRequest_t floorReq;
		if(userInput.direction == 'U')
			floorReq.direction = k_directionUp;
		else if(userInput.direction == 'D')
			floorReq.direction = k_directionDown;
		else if(userInput.floor >= '0' && userInput.floor <= '9')
			floorReq.bInsideRequest = true;
		else
			assert(!"invalid direction from IO");

		if(userInput.floor >= '0' && userInput.floor <= '9')
		{
			floorReq.floorNumber = userInput.floor - '0';
		}
		else
			assert(!"invalid floor from IO");

		floorRequestVectProtector_consumer.Wait();
		m_floorRequestVect.push_back(floorReq);
		floorRequestVectProtector_producer.Signal();
		



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
	} while(1);
	return 0;
}

int Dispatcher::main()
{
	
	CPipe IoToDispatcher_pipeline("DispatcherCommands", 1024);
	std::vector<ClassThread<Dispatcher>*> collectElevatorStatusVect;
	int yArray[100];
	std::vector<ClassThread<Dispatcher>*> dispatcherToElevatorVect;
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
		xArray[i-1] = i+10;
		if( i - 1 >= 0)
		{	
			ClassThread<Dispatcher>* pDispatcherToElevator= new ClassThread<Dispatcher>(this,&Dispatcher::DispatcherToElevator, ACTIVE, &xArray[i-1]);
			dispatcherToElevatorVect.push_back(pDispatcherToElevator);
			CSemaphore dispatcherLocalElevatorStatus_consumer("DispatcherLocalElevatorStatus"+elevatorNumberStr+"Consumer",1,1);
			CSemaphore dispatcherLocalElevatorStatus_producer("DispatcherLocalElevatorStatus"+elevatorNumberStr+"Producer",0,1);
		}
	}//add delete in for the pointers in the vectors
	ClassThread<Dispatcher>	 IoToDispatcherPipeline(this,&Dispatcher::ReadFromIoToDispatcherPipeline,ACTIVE, NULL);

	
	for( int i = 1; i <= m_numberOfElevators; i++)
	{
		yArray[i-1] = i; //may be able to use xArray
		if( i - 1 >= 0)
		{	
			ClassThread<Dispatcher>* pCollectElevatorStatus= new ClassThread<Dispatcher>(this,&Dispatcher::CollectElevatorStatus, ACTIVE, &yArray[i-1]);
			collectElevatorStatusVect.push_back(pCollectElevatorStatus);
			//printf("Created %d threads in IOProgram\n", i);
			//Sleep(500);
		}// FIXME add delete in for the pointers in the vectors //add waitfor thread at the end
	}

	FloorRequestVect_t floorRequestQueue;
	do{

		if(floorRequestVectProtector_producer.Read() > 0) //Check to see if there is a new command
		{
			// read from the member variable storing the queue of floor requests to be used for FS value calculations
			floorRequestVectProtector_producer.Wait();
			floorRequestQueue = m_floorRequestVect;
		//	numberDirection = m_userInputData.direction - '0';
		//	if(numberDirection <= m_numberOfElevators && numberDirection > 0) //if the command is sent 
		//		m_pElevatorCommands[numberDirection-1]->Write(&m_userInputData, sizeof(UserInputData_t));
			floorRequestVectProtector_consumer.Signal();
			
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
		
		if(m_bExit)
		{
			printf("breaking from dispatcher loop\n");
			break; 
		}
	} while(1);

	IoToDispatcherPipeline.WaitForThread();
	
	//May need this
	/*for( int i = 0; i < m_numberOfElevators; i++)
	{
		dispatcherToElevatorVect[i]->WaitForThread();
	}*/
	for( int i = 0; i < m_numberOfElevators; i++)
	{
		delete dispatcherToElevatorVect[i];
		delete collectElevatorStatusVect[i];
	}
	
	printf("Exiting dispatcher\n");
	return 0;
}