/*****************************************************
 * EECE 314 Assignment #1
 * Authors: Kieren Wou & Ryan Wong
 * Date: October 2013
 *****************************************************/
#include "Dispatcher.h"


Dispatcher::Dispatcher()
{}

Dispatcher::Dispatcher(IOProgramPtr_t pIoProgram)
	: m_pIoProgram(pIoProgram)
	, m_bExit(FALSE)
{}

// Thread to read pipeline with commands from IOProgram
int Dispatcher::ReadFromPipeline3(void *args)
{ 
	CPipe IoToDispatcher_pipeline("IoToDispatcherPipeline", 1024);
	UserInputData_t userInput;

	do{
			
		IoToDispatcher_pipeline.Read(&userInput, sizeof(UserInputData_t));
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

	CDataPool elevator1_datapool("Elevator1Status", sizeof(ElevatorStatus_t));
	CDataPool elevator2_datapool("Elevator2Status", sizeof(ElevatorStatus_t));
	
	CPipe ECommands1("Elevator1Commands", 1024);
	CPipe ECommands2("Elevator2Commands", 1024);
	CPipe IoToDispatcher_pipeline("DispatcherCommands", 1024);
	
	ClassThread<Dispatcher>	 IoToDispatcherPipeline(this,&Dispatcher::ReadFromPipeline3,ACTIVE, NULL);

	//ElevatorStatusPtr_t	elevator1Status = (ElevatorStatusPtr_t)(elevator1_datapool.LinkDataPool());
	//ElevatorStatusPtr_t	elevator2Status = (ElevatorStatusPtr_t)(elevator2_datapool.LinkDataPool());

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