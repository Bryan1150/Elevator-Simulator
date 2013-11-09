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
	}

	for(int i = 0; i < m_numberOfElevators; ++i)
	{
		m_pElevatorStatus[i] = (ElevatorStatusPtr_t)(m_pElevatorDataPool[i]->LinkDataPool());		//link to datapools of elevator statuses
	}
}

IOProgram::~IOProgram()
{
	for(int i = 0; i < m_numberOfElevators; ++i)
	{
		delete m_pElevatorDataPool[i];
	}
	delete m_screenMutex;
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
					if((userInput.direction == 'U' && userInput.floor == '9') ||
						(userInput.direction == 'D' && userInput.floor == '0'))
						return false;
					else
						return true;
				}
				else if( (numberDirection <= m_numberOfElevators && numberDirection > 0) &&  //the command is being made from within the elevator
						  (userInput.floor <= '9' && userInput.floor >= '0' ) )
				{
					return true;
				}
				else if( (userInput.direction == 'C' || userInput.direction == 'F') &&   //the command is being made to detect or clear a fault
					(numberFloor <= m_numberOfElevators && numberFloor > 0) )
				{
					return true;
				}
				else if( userInput.direction == 'E' && userInput.floor == 'E') //the command is to terminate the simulation
				{
					return true;
				}
	}
	return false;
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

	printf("Elevator %d from IO\nDirection: %d\nDoor Status: %d\nFloor Number: %d\n", 
		elevatorNumber,
		elevatorStatus.direction,
		elevatorStatus.doorStatus,
		elevatorStatus.floorNumber);
	m_screenMutex->Signal();

}

int IOProgram::CollectElevatorStatus(void* args)
{
	std::stringstream ss;
	int elevatorId = *(int*)args;
	ss << elevatorId;
	std::string elevatorNumberStr = ss.str();

	CSemaphore elevatorToIO_consumer("Elevator"+elevatorNumberStr+"ToIOConsumer",1,1);
	CSemaphore elevatorToIO_producer("Elevator"+elevatorNumberStr+"ToIOProducer",0,1);
	CPipe IoToElevatorGraphics_pipeline("IoToElevatorGraphics"+elevatorNumberStr, 1024);/*******************************/
	
	do{
		if(elevatorToIO_producer.Read() > 0) // elevator 1 produced data
		{
			if(elevatorId-1 >= 0)
			{
				elevatorToIO_producer.Wait();

				m_localElevatorStatus[elevatorId-1] = *m_pElevatorStatus[elevatorId-1];
				OutputDebugString("IO Program has read incoming Elevator Status\n");

				elevatorToIO_consumer.Signal();
				
				UpdateElevatorStatus(m_localElevatorStatus[elevatorId-1],elevatorId);	//update visual for elevator 1

				IoToElevatorGraphics_pipeline.Write(&m_localElevatorStatus[elevatorId-1], sizeof(ElevatorStatus_t));/****************/
			}
		}
	} while(!m_exit);
	
	return 0;
}
int IOProgram::main()
{
	CPipe IoToDispatcher_pipeline(k_ioToDispatcherPipeline, 1024);	//initialize pipeline to receive data from IO program
	CPipe IoOutsideRequestsToElevatorGraphics_pipeline("IoOutsideRequestsToElevatorGraphics",1024);/*******************************/
	CPipe IoInsideRequestsToElevatorGraphics_pipeline("IoInsideRequestsToElevatorGraphics",1024);/*******************************/

	int keys_pressed = 0;											//count of number of keys pressed
	CMailbox DispatcherToIo_mailbox;								//mailbox for the IOprogram to receive messages from the dispatcher

	UserInputData_t userInput;										
	
	std::vector<ClassThread<IOProgram>*> collectElevatorStatusVect; //vector to hold pointers for threads collecting elevator statuses
	int elevatorNumberArray[10];												//used to pass in elevator number to the threads

	std::stringstream ss;
	ss << m_numberOfElevators;
	std::string elevatorNumberStr = ss.str();

	/*******************************/
	CProcess p1("Z:\\RTExamples\\EECE314\\Assignment1_Elevators\\Debug\\ElevatorGraphics.exe "+elevatorNumberStr,	// pathlist to child program executable				
			NORMAL_PRIORITY_CLASS,			// priority
			OWN_WINDOW,						// process has its own window					
			ACTIVE							// process is active immediately
	) ;

	//initialize threads for collecting elevator statuses
	for( int i = 1; i <= m_numberOfElevators; i++)
	{
		elevatorNumberArray[i-1] = i;
		if( i - 1 >= 0)
		{	
			ClassThread<IOProgram>* pCollectElevatorStatus= new ClassThread<IOProgram>(this,&IOProgram::CollectElevatorStatus, ACTIVE, &elevatorNumberArray[i-1]);
			collectElevatorStatusVect.push_back(pCollectElevatorStatus);
		} // FIXME add delete in for the pointers in the vectors //add waitfor thread at the end
	}
	
	
	//Prompt user for commands
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

		if(keys_pressed == 2)
		{
			printf("\nEntered Test\n");
		
			printf("\nReceived two values\n");

			if(IsValidCommand(userInput))		//Check if the command was valid
			{
				printf("Sending commands\n");
				IoToDispatcher_pipeline.Write(&userInput, sizeof(UserInputData_t));
				if( userInput.direction == 'U' || userInput.direction == 'D')
					IoOutsideRequestsToElevatorGraphics_pipeline.Write(&userInput, sizeof(UserInputData_t));
				else if( userInput.direction <= '9' && userInput.direction >= '1')
					IoInsideRequestsToElevatorGraphics_pipeline.Write(&userInput,sizeof(UserInputData_t));
				else if( userInput.direction == 'E' && userInput.floor == 'E') //terminate the threads processing requests in graphics
				{
					IoOutsideRequestsToElevatorGraphics_pipeline.Write(&userInput, sizeof(UserInputData_t));
					IoInsideRequestsToElevatorGraphics_pipeline.Write(&userInput,sizeof(UserInputData_t));
				}
					
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


		if(DispatcherToIo_mailbox.TestForMessage()) //Check mailbox for messages from dispatcher
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


	for( int i = 0; i < m_numberOfElevators; i++)
	{
		collectElevatorStatusVect[i]->WaitForThread();
	}
	
	for( int i = 0; i < m_numberOfElevators; i++)
	{
		delete collectElevatorStatusVect[i];
	}

	return 0;
}