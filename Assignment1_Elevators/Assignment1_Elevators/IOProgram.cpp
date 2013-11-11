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

IOProgram::IOProgram(int numberOfElevators, std::string path) 
	: m_exit(FALSE)
	, m_numberOfElevators(numberOfElevators)
	, m_path(path)
{
	// mutex to protect writing to the console
	m_screenMutex = new CMutex("PrintToScreen");
	std::stringstream ss;
	std::string elevatorNumber;

	// intialized data pools to link with elevator status data pools
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

// This function checks to see if the user input is valid, Return true if it is valid and false if it is not valid
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
		MOVE_CURSOR(0,elevatorNumber*6+m_numberOfElevators);

	printf("Elevator %d\n", elevatorNumber);
	printf("Status: ");
	if( elevatorStatus.bFault)
		printf("Out of Service\n");
	else if( !elevatorStatus.bFault)
		printf("In Service    \n");
	printf("Direction: ");
	if(elevatorStatus.direction == k_directionIdle)
		printf("Idle\n");
	else if( elevatorStatus.direction == k_directionUp)
		printf("Up  \n");
	else if( elevatorStatus.direction == k_directionDown)
		printf("Down\n");
	printf("Door Status: ");
	if( elevatorStatus.doorStatus == k_doorOpen)
		printf("Open  \n");
	else if( elevatorStatus.doorStatus == k_doorClosed)
		printf("Closed\n");
	printf("Floor Number: %d\n",elevatorStatus.floorNumber);
	m_screenMutex->Signal();

}

int IOProgram::CollectElevatorStatus(void* args)
{
	// store elevator ID as a string in order to match the associating producer/consumer semaphores
	std::stringstream ss;
	int elevatorId = *(int*)args;
	ss << elevatorId;
	std::string elevatorNumberStr = ss.str();

	// producer/consumer semaphores for elevator status data pools
	CSemaphore elevatorToIO_consumer("Elevator"+elevatorNumberStr+"ToIOConsumer",1,1);
	CSemaphore elevatorToIO_producer("Elevator"+elevatorNumberStr+"ToIOProducer",0,1);
	CMutex IoLocalElevatorStatus("IoLocalElevatorStatus"); // mutex to protect local elevator status structure
	CPipe IoToElevatorGraphics_pipeline("IoToElevatorGraphics"+elevatorNumberStr, 1024);// pipeline to send elevator statuses to graphics process
	
	do{
		if(elevatorToIO_producer.Read() > 0) // elevator produced data
		{
			if(elevatorId-1 >= 0)
			{
				// store elevator statuses into local structures
				elevatorToIO_producer.Wait();
				IoLocalElevatorStatus.Wait();

				m_localElevatorStatus[elevatorId-1] = *m_pElevatorStatus[elevatorId-1];

				elevatorToIO_consumer.Signal();
				IoLocalElevatorStatus.Signal();

				UpdateElevatorStatus(m_localElevatorStatus[elevatorId-1],elevatorId);	//update text information for elevator

				// send elevator statuses to the graphics to update
				IoToElevatorGraphics_pipeline.Write(&m_localElevatorStatus[elevatorId-1], sizeof(ElevatorStatus_t)); /****************/
			}
		}
	} while(!m_exit);

	return 0;
}

int IOProgram::main()
{
	CPipe IoToDispatcher_pipeline(k_ioToDispatcherPipeline, 1024);	//initialize pipeline to receive data from IO program
	CPipe IoOutsideRequestsToElevatorGraphics_pipeline("IoOutsideRequestsToElevatorGraphics",1024); // pipeline to send outside floor requests to graphics process
	CPipe IoInsideRequestsToElevatorGraphics_pipeline("IoInsideRequestsToElevatorGraphics",1024); // pipeline to send inside floor requests to graphics process
	CMutex IoLocalElevatorStatus("IoLocalElevatorStatus");			// mutex to protect local structure of elevator statuses


	int keys_pressed = 0;											//count of number of keys pressed
	CMailbox DispatcherToIo_mailbox;								//mailbox for the IOprogram to receive messages from the dispatcher

	UserInputData_t userInput;										// struct for holding user input
	
	std::vector<ClassThread<IOProgram>*> collectElevatorStatusVect; //vector to hold pointers for threads collecting elevator statuses
	int elevatorNumberArray[10];												//used to pass in elevator number to the threads

	std::stringstream ss;						// convert number of elevators into a string							
	ss << m_numberOfElevators;
	std::string elevatorNumberStr = ss.str();

	static const std::string k_defaultPath("Z:\\RTExamples\\EECE314\\Assignment1_Elevators\\Debug\\ElevatorGraphics.exe");
	std::string elevatorGraphicsPath = m_path == "def" ? k_defaultPath : m_path;
	CProcess p1(elevatorGraphicsPath+" "+elevatorNumberStr,	// pathlist to child program executable				
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
		} 
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

			if(IsValidCommand(userInput))		//Check if the command was valid
			{ 
			
				IoToDispatcher_pipeline.Write(&userInput, sizeof(UserInputData_t));
				
				// send floor request to corresponding graphics pipline if it is an outside request
				if( userInput.direction == 'U' || userInput.direction == 'D') 
				{
					IoOutsideRequestsToElevatorGraphics_pipeline.Write(&userInput, sizeof(UserInputData_t));
				}

				// send floor request to corresponding graphics pipline if it is an insde request
				else if( userInput.direction <= m_numberOfElevators+'0' && userInput.direction >= 0 +'0') 
				{
					IoLocalElevatorStatus.Wait();
					
					// check to see if there is a fault in this elevator; if there is, do not send the request
					if(m_localElevatorStatus[userInput.direction-'0'-1].bFault == false) 
						IoInsideRequestsToElevatorGraphics_pipeline.Write(&userInput, sizeof(UserInputData_t));
					
					IoLocalElevatorStatus.Signal();				
				}

				//terminate the threads processing requests in graphics
				else if( userInput.direction == 'E' && userInput.floor == 'E') 
				{
					IoOutsideRequestsToElevatorGraphics_pipeline.Write(&userInput, sizeof(UserInputData_t));
					IoInsideRequestsToElevatorGraphics_pipeline.Write(&userInput, sizeof(UserInputData_t));
				}
			}
			else
			{
				m_screenMutex->Wait();
				MOVE_CURSOR(0,1);
				printf("Error: invalid command\n"); // display error message for invalid commands
				m_screenMutex->Signal();
			}
			keys_pressed = 0; //reset the number of keys pressed value
			Sleep(500);
			ClearLines(5);

			m_screenMutex->Wait();
			MOVE_CURSOR(0,0);
			printf("Enter Commands: ");
			m_screenMutex->Signal();
		}	


		if(DispatcherToIo_mailbox.TestForMessage()) //Check mailbox for messages from dispatcher
		{		
			UINT message = DispatcherToIo_mailbox.GetMessage() ;
			m_screenMutex->Wait();
			MOVE_CURSOR(0,1);
			printf("Message = %d\n", message);
			m_screenMutex->Signal();
			if(message == k_terminateSimulation) // if message matches termination flag, set necessary conditions	
			{			
				m_screenMutex->Wait();
				MOVE_CURSOR(0,2);
				printf("Received TERMINATE Message in IO.....\n") ;
				m_exit = TRUE;
				m_screenMutex->Signal();
				//printf("Set m_exit to True in IO\n");
			}
		}
		
	} while(!m_exit);

	// Wait for child threads to terminate
	p1.WaitForProcess();
	for( int i = 0; i < m_numberOfElevators; i++)
	{
		collectElevatorStatusVect[i]->WaitForThread();
	}
	
	// deletes pointers for child threads
	for( int i = 0; i < m_numberOfElevators; i++)
	{
		delete collectElevatorStatusVect[i];
	}
	m_screenMutex->Wait();
	MOVE_CURSOR(0,3);
	printf("Exiting  IO.....\n") ;
	m_screenMutex->Signal();
	return 0;
}