/*****************************************************
 * EECE 314 Assignment #1
 * Date: October 2013
 * Authors: 
 *	 Kieren Wou	54724117
 *	 Ryan Wong	47307103
 *****************************************************/
#include <conio.h>
#include <sstream>
#include <vector>

#include "IOProgram.h"
#include "Dispatcher.h"


IOProgram::IOProgram() : m_exit(FALSE)
{
	m_screenMutex = new CMutex("PrintToScreen");
}

IOProgram::IOProgram(int numberOfElevators) 
	: m_exit(FALSE)
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
}
bool IOProgram::IsValidCommand(UserInputData_t userInput) const
{
	int numberDirection = userInput.direction - '0'; 
	int numberFloor = userInput.floor - '0'; 
	if( (userInput.direction == 'U' || userInput.direction == 'D' || userInput.direction == 'E' ||
		(numberDirection <= m_numberOfElevators && numberDirection > 0) || userInput.direction == 'F' || 
		userInput.direction == 'C' ) )
	{
			
				if( (userInput.direction == 'U' || userInput.direction == 'D') &&  // The command is for calling the elevator
					(userInput.floor <= '9' && userInput.floor >= '0' ) )
				{
					return TRUE;
				}
				else if( (numberDirection <= m_numberOfElevators && numberDirection > 0) &&  //the command is being made from within the elevator
						  (userInput.floor <= '9' && userInput.floor >= '0' ) )
				{
					return TRUE;
				}
				else if( (userInput.direction == 'C' || userInput.direction == 'F') &&   //the command is being made to detect or clear a fault
					(numberFloor <= m_numberOfElevators && numberFloor > 0) )
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

	m_screenMutex->Wait();
	for(int i = 0; i < lines; ++i)
	{
		MOVE_CURSOR(0,i);
		for(int i = 0; i < 256; ++i)
			printf(" ");
	}
	m_screenMutex->Signal();
}

void IOProgram::UpdateElevatorStatus(ElevatorStatus_t elevatorStatus, int elevatorNumber) const
{
	m_screenMutex->Wait();	
		MOVE_CURSOR(0,elevatorNumber*5+5);

	printf("Elevator %d\n"
		"Direction: %d\n"
		"Door Status: %d\n"
		"Floor Number: %d\n", elevatorNumber,elevatorStatus.direction,elevatorStatus.doorStatus,elevatorStatus.floorNumber);
	m_screenMutex->Signal();

}
int IOProgram::CollectElevatorStatus(void* args)
{
	std::stringstream ss;
	int x = *(int*)args;
	ss << x;
	std::string elevatorNumber = ss.str();

	CSemaphore elevatorToIO_consumer("Elevator"+elevatorNumber+"ToIOConsumer",1,1);
	CSemaphore elevatorToIO_producer("Elevator"+elevatorNumber+"ToIOProducer",0,1);
	do{
		if(elevatorToIO_producer.Read() > 0) // elevator 1 produced data
			{
			  if(x-1>=0)
			  {
				elevatorToIO_producer.Wait();
				//printf("Copying data from elevator1Status in IO program\n");
				m_localElevatorStatus[x-1].direction = m_pElevatorStatus[x-1]->direction;
				m_localElevatorStatus[x-1].doorStatus = m_pElevatorStatus[x-1]->doorStatus;
				m_localElevatorStatus[x-1].floorNumber = m_pElevatorStatus[x-1]->floorNumber;
				elevatorToIO_consumer.Signal();
				UpdateElevatorStatus(m_localElevatorStatus[x-1],x);	//update visual for elevator 1
			  }
			}
	}while(!m_exit);
	return 0;
}
int IOProgram::main()
{
	
	
	
	/*CDataPool elevator1_datapool("Elevator1Status", sizeof(ElevatorStatus_t));
	CDataPool elevator2_datapool("Elevator2Status", sizeof(ElevatorStatus_t));*/
	
	CSemaphore elevator1ToIO_consumer("Elevator1ToIOConsumer",1,1);
	CSemaphore elevator1ToIO_producer("Elevator1ToIOProducer",0,1);
	CSemaphore elevator2ToIO_consumer("Elevator2ToIOConsumer",1,1);
	CSemaphore elevator2ToIO_producer("Elevator2ToIOProducer",0,1);

	CPipe IoToDispatcher_pipeline(k_ioToDispatcherPipeline, 1024);
	int keys_pressed = 0;
	CMailbox DispatcherToIo_mailbox;

	UserInputData_t userInput; // Struct for holdering user input
	
	
	std::vector<ClassThread<IOProgram>*> collectElevatorStatusVect;
	int xArray[100];


	for( int i = 1; i <= m_numberOfElevators; i++)
	{
		xArray[i-1] = i;
		if( i - 1 >= 0)
		{	
			ClassThread<IOProgram>* pCollectElevatorStatus= new ClassThread<IOProgram>(this,&IOProgram::CollectElevatorStatus, ACTIVE, &xArray[i-1]);
			collectElevatorStatusVect.push_back(pCollectElevatorStatus);
			//printf("Created %d threads in IOProgram\n", i);
			//Sleep(500);
		}//add delete in for the pointers in the vectors //add waitfor thread at the end
	}
	

	/*ElevatorStatusPtr_t	elevator1Status = 
	ElevatorStatusPtr_t	elevator2Status = (ElevatorStatusPtr_t)(elevator2_datapool.LinkDataPool());*/

	//ElevatorStatus_t localElevator1Status, localElevator2Status; // local data structures of elevator status

	m_screenMutex->Wait();
	MOVE_CURSOR(0,0);
	printf("Enter Commands: ");
	m_screenMutex->Signal();
	do{
		if(TEST_FOR_KEYBOARD())		//Test for keyboard inputs
		{
			if(keys_pressed == 0)
			{
				userInput.direction = _getch();
				m_screenMutex->Wait();
				MOVE_CURSOR(17,0);
				_putch(userInput.direction);
				m_screenMutex->Signal();
			}
			else
			{
				userInput.floor = _getch();
				m_screenMutex->Wait();
				MOVE_CURSOR(18,0);
				_putch(userInput.floor);
				m_screenMutex->Signal();
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
		//if(elevator1ToIO_producer.Read() > 0) // elevator 1 produced data
		//{
		//	
		//	elevator1ToIO_producer.Wait();
		//	//printf("Copying data from elevator1Status in IO program\n");
		//	m_localElevatorStatus[0].direction = m_pElevatorStatus[0]->direction;
		//	m_localElevatorStatus[0].doorStatus = m_pElevatorStatus[0]->doorStatus;
		//	m_localElevatorStatus[0].floorNumber = m_pElevatorStatus[0]->floorNumber;
		//	elevator1ToIO_consumer.Signal();
		//	UpdateElevatorStatus(m_localElevatorStatus[0],1);	//update visual for elevator 1

		//}

		//if(elevator2ToIO_producer.Read() > 0) // elevator 2 produced data
		//{
		//	
		//	elevator2ToIO_producer.Wait();
		//	//printf("Copying data from elevator2Status in IO program\n");
		//	m_localElevatorStatus[1].direction = m_pElevatorStatus[1]->direction;
		//	m_localElevatorStatus[1].doorStatus = m_pElevatorStatus[1]->doorStatus;
		//	m_localElevatorStatus[1].floorNumber = m_pElevatorStatus[1]->floorNumber;
		//	elevator2ToIO_consumer.Signal();
		//	UpdateElevatorStatus(m_localElevatorStatus[1],2);	//update visual for elevator 2

		//}
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
					m_screenMutex->Wait();
					MOVE_CURSOR(0,3);
					printf("Received TERMINATE Message in IO.....\n") ;
					m_exit = TRUE;
					m_screenMutex->Signal();
					//printf("Set m_exit to True in IO\n");
				}
			
		}
	
		
	} while(!m_exit);

	
	//printf("Left the main loop of IOprogram\n");

	//delete elevator1Status;
	//delete elevator2Status;
	for( int i = 0; i < m_numberOfElevators; i++)
	{
		collectElevatorStatusVect[i]->WaitForThread();
	}
	for( int i = 0; i < m_numberOfElevators; i++)
	{
		//printf("Entered to Delete collectElevatorStatus %d\n", i+1);
		delete collectElevatorStatusVect[i];
		//Sleep(100);
		//printf("Deleted collectElevatorStatus %d\n", i+1);
		
	}
		printf("Exiting from IO\n");
	return 0;
}