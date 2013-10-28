/*****************************************************
 * EECE 314 Assignment #1
 * Authors: Kieren Wou & Ryan Wong
 * Date: October 2013
 *****************************************************/
#include <conio.h>

#include "IOProgram.h"
#include "Dispatcher.h"


IOProgram::IOProgram() : m_exit(FALSE)
{}

int IOProgram::IsValidCommand(UserInputData_t userInput) const
{
	int isValid = 0;

	if( (userInput.direction == 'U' || userInput.direction == 'D' || userInput.direction == 'E' ||
		userInput.direction == '1' || userInput.direction == '2' || userInput.direction == 'F' || 
		userInput.direction == 'C' ) )
	{
			
				if( (userInput.direction == 'U' || userInput.direction == 'D') &&  // The command is for calling the elevator
					(userInput.floor <= '9' && userInput.floor >= '0' ) )
				{
					isValid = 1;
				}
				else if( (userInput.direction == '1' || userInput.direction == '2') &&  //the command is being made from within the elevator
						  (userInput.floor <= '9' && userInput.floor >= '0' ) )
				{
					isValid = 1;
				}
				else if( (userInput.direction == 'C' || userInput.direction == 'F') &&   //the command is being made to detect or clear a fault
					(userInput.floor == '1' || userInput.floor == '2') )
				{
					isValid = 1;
				}
				else if( userInput.direction == 'E' && userInput.floor == 'E') //the command is to terminate the simluation
				{
					isValid =  1;
				}
	}
	return isValid;
}

int IOProgram::main()
{
	CDataPool elevator1_datapool("Elevator1Status", sizeof(ElevatorStatus_t));
	CDataPool elevator2_datapool("Elevator2Status", sizeof(ElevatorStatus_t));
	
	CPipe IoToDispatcher_pipeline("IoToDispatcherPipeline", 1024);
	int keys_pressed = 0;
	CMailbox DispatcherToIo_mailbox;

	UserInputData_t userInput;

	//ElevatorStatusPtr_t	elevator1Status = (ElevatorStatusPtr_t)(elevator1_datapool.LinkDataPool());
	//ElevatorStatusPtr_t	elevator2Status = (ElevatorStatusPtr_t)(elevator2_datapool.LinkDataPool());
	
	do{
		if(TEST_FOR_KEYBOARD())		//Test for keyboard inputs
		{
			if(keys_pressed == 0)
			{
				userInput.direction = _getch();
				_putch(userInput.direction);
			}
			else
			{
				userInput.floor = _getch();
				_putch(userInput.floor);
			}
			++keys_pressed;
		}
		//printf("keys pressed = %d\n", keys_pressed);
		if(keys_pressed == 2)
		{
				printf("\nEntered Test\n");
		
				printf("\nReceived two values\n");
		
				if(IsValidCommand(userInput))		//Check if the command was valid
				{
					printf("Sending commands\n");
					IoToDispatcher_pipeline.Write(&userInput, sizeof(UserInputData_t));
			
				}
				else
				{
					printf("Error: invalid command\n");
				}
				keys_pressed = 0; //reset the number of keys pressed value
		}	
		/*else
		{
				printf("Error: invalid command\n");
		}*/
			
		


		if(DispatcherToIo_mailbox.TestForMessage()) 
		{		
				UINT message = DispatcherToIo_mailbox.GetMessage() ;	
				printf("Message = %d\n", message);
				if(message == k_terminateSimulation)	
				{			
					printf("Received TERMINATE Message.....\n") ;
					m_exit = TRUE;
				}
			
		}
	
		
	} while(!m_exit);

	

	printf("Exiting from IO\n");

	//delete elevator1Status;
	//delete elevator2Status;

	return 0;
}