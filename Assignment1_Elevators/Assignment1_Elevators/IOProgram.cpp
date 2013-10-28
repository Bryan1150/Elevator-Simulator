/*****************************************************
 * EECE 314 Assignment #1
 * Authors: Kieren Wou & Ryan Wong
 * Date: October 2013
 *****************************************************/
#include <conio.h>

#include "IOProgram.h"
#include "Dispatcher.h"


IOProgram::IOProgram()
{}


int IOProgram::main()
{
	CDataPool elevator1_datapool("Elevator1Status", sizeof(ElevatorStatus_t));
	CDataPool elevator2_datapool("Elevator2Status", sizeof(ElevatorStatus_t));
	
	CPipe IoToDispatcher_pipeline("IoToDispatcherPipeline", 1024);
	
	CMailbox DispatcherToIo_mailbox;

	UserInputData_t userInput;

	//ElevatorStatusPtr_t	elevator1Status = (ElevatorStatusPtr_t)(elevator1_datapool.LinkDataPool());
	//ElevatorStatusPtr_t	elevator2Status = (ElevatorStatusPtr_t)(elevator2_datapool.LinkDataPool());
	
	do{
		userInput.direction = _getch();
		_putch(userInput.direction);
		userInput.floor = _getch();
		_putch(userInput.floor);

		printf("\nReceived two values\n");
		
		if( userInput.direction == 'U' || userInput.direction == 'D' || userInput.direction == 'E')
		{
			
			if( ((userInput.direction == 'U' || userInput.direction == 'D') && 
				(userInput.floor <= '9' && userInput.floor >= '0' )) || 
				(userInput.direction == 'E' && userInput.floor == 'E') )
			{
				printf("Sending commands\n");
				IoToDispatcher_pipeline.Write(&userInput, sizeof(UserInputData_t));
				
				if(userInput.direction == 'E' && userInput.floor == 'E')
					break;
			}
			else
			{
				printf("Error: invalid command\n");
			}
		
		}
		
		else
		{
			printf("Error: invalid command\n");
		}
	
	} while(1);

	do{
		if(DispatcherToIo_mailbox.TestForMessage()) 
		{		
				UINT message = DispatcherToIo_mailbox.GetMessage() ;	
				printf("Message = %d\n", message);
				if(message == k_terminateSimulation)	
				{			
					printf("Received TERMINATE Message.....\n") ;
					break;
				}
			
		}
	} while(1);

	printf("Exiting from IO\n");

	//delete elevator1Status;
	//delete elevator2Status;

	return 0;
}