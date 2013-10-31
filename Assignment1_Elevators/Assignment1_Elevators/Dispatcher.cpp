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
	m_screenMutex = new CMutex("PrintToScreen");
	std::stringstream ss;
	std::string elevatorNumber;
	for(int i = 1; i <= m_numberOfElevators; ++i)
	{
		ss << i;
		elevatorNumber = ss.str();
		m_pElevatorDataPool[i-1] = new CDataPool("Elevator"+elevatorNumber+"Status",sizeof(ElevatorStatus_t)); 
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
	/*CDataPool elevator1_datapool(k_elevator1StatusDataPool, sizeof(ElevatorStatus_t));
	CDataPool elevator2_datapool(k_elevator2StatusDataPool, sizeof(ElevatorStatus_t));

	m_pElevator1Status = (ElevatorStatusPtr_t)(elevator1_datapool.LinkDataPool());
	m_pElevator2Status = (ElevatorStatusPtr_t)(elevator2_datapool.LinkDataPool());*/
}

//Thread to write commands to pipeline 1 (elevator 1)
int Dispatcher::DispatcherToElevator(void* args)
{
	
	std::stringstream ss;
	int x = *(int*)args;
	ss << x;
	std::string elevatorNumber = ss.str();
	CPipe elevatorCommands1("Elevator"+elevatorNumber+"Commands", 1024);
	do{

			//printf("%c\n",elevatorNumber);
	}while(1);
	return 0;
}

//Thread to write commands to pipeline 2 (elevator 2)
int Dispatcher::WriteToPipeline2(void* args)
{
	CPipe ECommands2(k_elevator1Commands, 1024);

	return 0;
}

// Thread to read pipeline with commands from IOProgram
int Dispatcher::ReadFromPipeline3(void *args)
{ 
	CPipe IoToDispatcher_pipeline(k_ioToDispatcherPipeline, 1024);
	UserInputData_t userInput;

	do{
			
		IoToDispatcher_pipeline.Read(&userInput, sizeof(UserInputData_t));
		MOVE_CURSOR(0,1);
		printf("Direction = %c and Floor = %c\n", userInput.direction,userInput.floor);

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

	/*CDataPool elevator1_datapool("Elevator1Status", sizeof(ElevatorStatus_t));
	CDataPool elevator2_datapool("Elevator2Status", sizeof(ElevatorStatus_t));*/

	/*CSemaphore dispatcherToElevator1_consumer("DispatcherToElevator1Consumer",1,1);
	CSemaphore dispatcherToElevator1_producer("DispatcherToElevator1Producer",0,1);
	CSemaphore dispatcherToElevator2_consumer("DispatcherToElevator2Consumer",1,1);
	CSemaphore dispatcherToElevator2_producer("DispatcherToElevator2Producer",0,1);*/

	
	CPipe IoToDispatcher_pipeline("DispatcherCommands", 1024);
	
	std::vector<ClassThread<Dispatcher>*> dispatcherToElevatorVect;
	int xArray[100];


	for( int i = 1; i <= m_numberOfElevators; i++)
	{
		xArray[i-1] = i+10;
		if( i - 1 >= 0)
		{	
			ClassThread<Dispatcher>* pDispatcherToElevator= new ClassThread<Dispatcher>(this,&Dispatcher::DispatcherToElevator, ACTIVE, &xArray[i-1]);
			dispatcherToElevatorVect.push_back(pDispatcherToElevator);
		}
	}//add delete in for the pointers in the vectors
	ClassThread<Dispatcher>	 IoToDispatcherPipeline(this,&Dispatcher::ReadFromPipeline3,ACTIVE, NULL);

	//ClassThread<Dispatcher>	 DispatcherToElevator1Pipeline(this,&Dispatcher::DispatcherToElevator,ACTIVE, NULL);
	//ClassThread<Dispatcher>	 DispatcherToElevator2Pipeline(this,&Dispatcher::WriteToPipeline2,ACTIVE, NULL);

	/*ElevatorStatusPtr_t	elevator1Status = (ElevatorStatusPtr_t)(elevator1_datapool.LinkDataPool());
	ElevatorStatusPtr_t	elevator2Status = (ElevatorStatusPtr_t)(elevator2_datapool.LinkDataPool());*/

	do{
		if(m_bExit)
		{
			printf("breaking from dispatcher loop\n");
			break; 
		}
	} while(1);

	IoToDispatcherPipeline.WaitForThread();
	

	/*for( int i = 0; i < m_numberOfElevators; i++)
	{
		dispatcherToElevatorVect[i]->WaitForThread();
	}*/
	for( int i = 0; i < m_numberOfElevators; i++)
	{
		delete dispatcherToElevatorVect[i];
	}
	//delete elevator1_datapool;
	//delete elevator2_datapool;
	printf("Exiting dispatcher\n");
	return 0;
}