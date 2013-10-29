/*****************************************************
 * EECE 314 Assignment #1
 * Authors: Kieren Wou & Ryan Wong
 * Date: October 2013
 *****************************************************/
#include "Dispatcher.h"


Dispatcher::Dispatcher()
{}

Dispatcher::Dispatcher(IOProgramPtr_t pIoProgram)
	: m_pIoProgram(pIoProgram)  // have a pointer to the IOProgram object so that the dispatcher can post to its mailbox
	, m_bExit(FALSE)
{
	CDataPool elevator1_datapool(k_elevator1StatusDataPool, sizeof(ElevatorStatus_t));
	CDataPool elevator2_datapool(k_elevator2StatusDataPool, sizeof(ElevatorStatus_t));

	m_pElevator1Status = (ElevatorStatusPtr_t)(elevator1_datapool.LinkDataPool());
	m_pElevator2Status = (ElevatorStatusPtr_t)(elevator2_datapool.LinkDataPool());
}

//Thread to write commands to pipeline 1 (elevator 1)
int Dispatcher::WriteToPipeline1(void* args)
{
	CPipe ECommands1(k_elevator1Commands, 1024);
	

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

	CSemaphore dispatcherToElevator1_consumer("DispatcherToElevator1Consumer",1,1);
	CSemaphore dispatcherToElevator1_producer("DispatcherToElevator1Producer",0,1);
	CSemaphore dispatcherToElevator2_consumer("DispatcherToElevator2Consumer",1,1);
	CSemaphore dispatcherToElevator2_producer("DispatcherToElevator2Producer",0,1);

	
	CPipe IoToDispatcher_pipeline("DispatcherCommands", 1024);
	
	ClassThread<Dispatcher>	 IoToDispatcherPipeline(this,&Dispatcher::ReadFromPipeline3,ACTIVE, NULL);
	ClassThread<Dispatcher>	 DispatcherToElevator1Pipeline(this,&Dispatcher::WriteToPipeline1,ACTIVE, NULL);
	ClassThread<Dispatcher>	 DispatcherToElevator2Pipeline(this,&Dispatcher::WriteToPipeline2,ACTIVE, NULL);

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
	printf("Exiting dispatcher\n");

	//delete elevator1_datapool;
	//delete elevator2_datapool;

	return 0;
}