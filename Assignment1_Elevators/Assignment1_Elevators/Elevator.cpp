/*****************************************************
 * EECE 314 Assignment #1
 * Date: October 2013
 * Authors: 
 *	 Kieren Wou	54724117
 *	 Ryan Wong	47307103
 *****************************************************/
#include "Elevator.h"
#include <sstream>

Elevator::Elevator() 
	: m_elevatorNumber(0) // FIXME kwou: change default value
{
	m_pDispatcherToElevator_consumer = new CSemaphore("dispatcherToElevator_consumer",1,1);
	m_pDispatcherToElevator_producer = new CSemaphore("dispatcherToElevator_consumer",0,1);
	m_pElevatorToIO_consumer = new CSemaphore("elevatorToIO_consumer",1,1);
	m_pElevatorToIO_producer = new CSemaphore("elevatorToIO_producer",0,1);
	m_pElevatorDatapool = new CDataPool("dataPool",sizeof(ElevatorStatus_t));
	m_pElevatorCommands = new CPipe("elevatorCommands", 1024);

} 

Elevator::Elevator(int num) 
	: m_elevatorNumber(num)
{
	std::stringstream ss;
	std::string s_elevatorNumber;
	ss << m_elevatorNumber;
	s_elevatorNumber = ss.str();

	m_pDispatcherToElevator_consumer = new CSemaphore("DispatcherToElevator"+s_elevatorNumber+"Consumer",1,1);
	m_pDispatcherToElevator_producer = new CSemaphore("DispatcherToElevator"+s_elevatorNumber+"Producer",0,1);
	m_pElevatorToIO_consumer = new CSemaphore("Elevator"+s_elevatorNumber+"ToIOConsumer",1,1);
	m_pElevatorToIO_producer = new CSemaphore("Elevator"+s_elevatorNumber+"ToIOProducer",0,1);
	m_pElevatorDatapool = new CDataPool("Elevator"+s_elevatorNumber+"Status",sizeof(ElevatorStatus_t));
	m_pElevatorCommands = new CPipe("Elevator"+s_elevatorNumber+"Commands", 1024);
	m_pDispatcherFloorRequest = new CMutex("DispatcherFloorRequest");
	
}

Elevator::~Elevator()
{
	delete m_pDispatcherToElevator_consumer;
	delete m_pDispatcherToElevator_producer;
	delete m_pElevatorToIO_consumer;
	delete m_pElevatorToIO_producer;
	delete m_pElevatorDatapool;
	delete m_pElevatorCommands;
	delete m_pDispatcherFloorRequest;
}

//
int  Elevator::ReadCommandsFromPipeline(void* args)
{
	std::stringstream ss;
	ss << m_elevatorNumber;
	std::string elevatorNumber = ss.str();
	//UserInputData_t userInput;
	CPipe elevatorCommands("Elevator"+elevatorNumber+"Commands", 1024);
	do{
		//elevatorCommands.Read(&co
			//printf("%c\n",elevatorNumber);
	}while(1);
	return 0;
}
//Initiliazes the default values for the Elevator
void Elevator::UpdateElevatorStatus(ElevatorStatusPtr_t elevatorStatus, int direction, int doorStatus, int floorNumber) const
{
	
		m_pElevatorToIO_consumer->Wait();
		m_pDispatcherToElevator_consumer->Wait();
		elevatorStatus->direction = direction;
		elevatorStatus->doorStatus = doorStatus;
		elevatorStatus->floorNumber = floorNumber;
		m_pDispatcherToElevator_producer->Signal();
		m_pElevatorToIO_producer->Signal();
		
	/*if(m_elevatorNumber == 1)
		MOVE_CURSOR(10,10);
	else if(m_elevatorNumber == 2)
		MOVE_CURSOR(20,20);

	printf("Updated Elevator Status %d\n", m_elevatorNumber);
	Sleep(1000);*/
}

//Open or Close the Doors of the Elevator
void Elevator::ChangeDoorStatus(ElevatorStatusPtr_t elevatorStatus, int doorStatus) const
{
	if(doorStatus == 0)
		UpdateElevatorStatus(elevatorStatus, elevatorStatus->direction, k_closed, elevatorStatus->floorNumber); // close the doors
	else if(doorStatus == 1)
		UpdateElevatorStatus(elevatorStatus, elevatorStatus->direction, k_open, elevatorStatus->floorNumber); //open the doors
}

//Increment or decrement the floor number
void Elevator::ChangeFloorNumber(ElevatorStatusPtr_t elevatorStatus, int direction) const
{
	if(direction == 0)
		UpdateElevatorStatus(elevatorStatus, elevatorStatus->direction, elevatorStatus->doorStatus, elevatorStatus->floorNumber-1);
	else if(direction == 1)
		UpdateElevatorStatus(elevatorStatus, elevatorStatus->direction, elevatorStatus->doorStatus, elevatorStatus->floorNumber+1);
}

//Set the direction of the Elevator
void Elevator::SetElevatorDirection(ElevatorStatusPtr_t elevatorStatus, int direction) const
{
	if(direction == 0)
		UpdateElevatorStatus(elevatorStatus, k_idle, elevatorStatus->doorStatus, elevatorStatus->floorNumber); //set direction to idle
	else if(direction == 1)
		UpdateElevatorStatus(elevatorStatus, k_up, elevatorStatus->doorStatus, elevatorStatus->floorNumber); //set direction to up
	else if(direction == 2)
		UpdateElevatorStatus(elevatorStatus, k_down, elevatorStatus->doorStatus, elevatorStatus->floorNumber); //set direction to down
}






int Elevator::main()
{

	ElevatorStatusPtr_t	elevatorStatus = (ElevatorStatusPtr_t)(m_pElevatorDatapool->LinkDataPool());
	ClassThread<Elevator> getCommandsFromPipeline(this,&Elevator::ReadCommandsFromPipeline,ACTIVE,NULL);
	//int floorNumber;
	

	if(m_elevatorNumber == 1)
		UpdateElevatorStatus(elevatorStatus, k_idle, k_closed, 0); // initlialize elevator

	else if(m_elevatorNumber == 2)
		UpdateElevatorStatus(elevatorStatus, k_up, k_open, 9); 
	else if(m_elevatorNumber == 3)
		UpdateElevatorStatus(elevatorStatus, k_up, k_closed, 4); 

	Sleep(1000);
	do{

		Sleep(1000);
		if(elevatorStatus->floorNumber < k_maxFloorNumber && m_elevatorNumber == 1)
			UpdateElevatorStatus(elevatorStatus, elevatorStatus->direction, elevatorStatus->doorStatus, elevatorStatus->floorNumber+1);

		if(elevatorStatus->floorNumber > k_minFloorNumber && m_elevatorNumber == 2)
			UpdateElevatorStatus(elevatorStatus, elevatorStatus->direction, elevatorStatus->doorStatus, elevatorStatus->floorNumber-1);
			//floorNumber = (elevatorStatus->floorNumber)+1;
	
		if(elevatorStatus->floorNumber > k_minFloorNumber && m_elevatorNumber == 3){
			Sleep(500);
			UpdateElevatorStatus(elevatorStatus, elevatorStatus->direction, elevatorStatus->doorStatus, elevatorStatus->floorNumber-1);
		}

	}while(1);
	//delete elevator1Status;
	//delete elevator2Status;
	return 0;
}