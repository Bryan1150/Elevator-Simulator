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

bool IOProgram::IsValidCommand(UserInputData_t userInput) const
{
	if( (userInput.direction == 'U' || userInput.direction == 'D' || userInput.direction == 'E' ||
		userInput.direction == '1' || userInput.direction == '2' || userInput.direction == 'F' || 
		userInput.direction == 'C' ) )
	{
			
				if( (userInput.direction == 'U' || userInput.direction == 'D') &&  // The command is for calling the elevator
					(userInput.floor <= '9' && userInput.floor >= '0' ) )
				{
					return TRUE;
				}
				else if( (userInput.direction == '1' || userInput.direction == '2') &&  //the command is being made from within the elevator
						  (userInput.floor <= '9' && userInput.floor >= '0' ) )
				{
					return TRUE;
				}
				else if( (userInput.direction == 'C' || userInput.direction == 'F') &&   //the command is being made to detect or clear a fault
					(userInput.floor == '1' || userInput.floor == '2') )
				{
					return TRUE;
				}
				else if( userInput.direction == 'E' && userInput.floor == 'E') //the command is to terminate the simluation
				{
					return TRUE;
				}
	}
	return FALSE;
}

//This function is to clear lines in the console
void IOProgram::ClearLines(int lines) const
{
	CMutex	screenMutex("PrintToScreen");
	screenMutex.Wait();
	for(int i = 0; i < lines; ++i)
	{
		MOVE_CURSOR(0,i);
		for(int i = 0; i < 256; ++i)
			printf(" ");
	}
	screenMutex.Signal();
}

void IOProgram::UpdateElevatorStatus(ElevatorStatus_t elevatorStatus, int elevatorNumber) const
{
	if(elevatorNumber == 1)
		MOVE_CURSOR(0,12);
	else if(elevatorNumber == 2)
		MOVE_CURSOR(0,20);

	printf("Elevator %d\n"
		"Direction: %d\n"
		"Door Status: %d\n"
		"Floor Number: %d\n", elevatorNumber,elevatorStatus.direction,elevatorStatus.doorStatus,elevatorStatus.floorNumber);


}
int IOProgram::main()
{
	CDataPool elevator1_datapool("Elevator1Status", sizeof(ElevatorStatus_t));
	CDataPool elevator2_datapool("Elevator2Status", sizeof(ElevatorStatus_t));
	
	CSemaphore elevator1ToIO_consumer("Elevator1ToIOConsumer",1,1);
	CSemaphore elevator1ToIO_producer("Elevator1ToIOProducer",0,1);
	CSemaphore elevator2ToIO_consumer("Elevator2ToIOConsumer",1,1);
	CSemaphore elevator2ToIO_producer("Elevator2ToIOProducer",0,1);

	CPipe IoToDispatcher_pipeline("IoToDispatcherPipeline", 1024);
	int keys_pressed = 0;
	CMailbox DispatcherToIo_mailbox;

	UserInputData_t userInput; // Struct for holdering user input

	ElevatorStatusPtr_t	elevator1Status = (ElevatorStatusPtr_t)(elevator1_datapool.LinkDataPool());		//link to datapools of elevator statuses
	ElevatorStatusPtr_t	elevator2Status = (ElevatorStatusPtr_t)(elevator2_datapool.LinkDataPool());

	ElevatorStatus_t localElevator1Status, localElevator2Status; // local data structures of elevator status

	MOVE_CURSOR(0,0);
	printf("Enter Commands: ");
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
				Sleep(500);
				ClearLines(5);
				MOVE_CURSOR(0,0);
				printf("Enter Commands: ");
		}

		//printf("Elevator 1: Direction=%d , Status=%d, Floor Number =%d\n",elevator1Status->direction,elevator1Status->doorStatus,elevator1Status->floorNumber);
		//printf("Elevator 2: Direction=%d , Status=%d, Floor Number =%d\n",elevator2Status->direction,elevator2Status->doorStatus,elevator2Status->floorNumber);
		if(elevator1ToIO_producer.Read() > 0) // elevator 1 produced data
		{
			
			elevator1ToIO_producer.Wait();
			//printf("Copying data from elevator1Status in IO program\n");
			localElevator1Status.direction = elevator1Status->direction;
			localElevator1Status.doorStatus = elevator1Status->doorStatus;
			localElevator1Status.floorNumber = elevator1Status->floorNumber;
			elevator1ToIO_consumer.Signal();
			UpdateElevatorStatus(localElevator1Status,1);	//update visual for elevator 1

		}

		if(elevator2ToIO_producer.Read() > 0) // elevator 2 produced data
		{
			
			elevator2ToIO_producer.Wait();
			//printf("Copying data from elevator2Status in IO program\n");
			localElevator2Status.direction = elevator2Status->direction;
			localElevator2Status.doorStatus = elevator2Status->doorStatus;
			localElevator2Status.floorNumber = elevator2Status->floorNumber;
			elevator2ToIO_consumer.Signal();
			UpdateElevatorStatus(localElevator2Status,2);	//update visual for elevator 2

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