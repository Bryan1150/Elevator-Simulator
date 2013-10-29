/*****************************************************
 * EECE 314 Assignment #1
 * Authors: Kieren Wou & Ryan Wong
 * Date: October 2013
 *****************************************************/
#include "Elevator.h"


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

Elevator::Elevator(int num, InterprocessCommTypeNames_t interprocessCommTypeNames) 
	: m_elevatorNumber(num)
{
	m_pDispatcherToElevator_consumer = new CSemaphore(interprocessCommTypeNames.dispatcherToElevator_consumer,1,1);
	m_pDispatcherToElevator_producer = new CSemaphore(interprocessCommTypeNames.dispatcherToElevator_consumer,0,1);
	m_pElevatorToIO_consumer = new CSemaphore(interprocessCommTypeNames.elevatorToIO_consumer,1,1);
	m_pElevatorToIO_producer = new CSemaphore(interprocessCommTypeNames.elevatorToIO_producer,0,1);
	m_pElevatorDatapool = new CDataPool(interprocessCommTypeNames.dataPool,sizeof(ElevatorStatus_t));
	m_pElevatorCommands = new CPipe(interprocessCommTypeNames.elevatorCommands, 1024);
	
}

Elevator::~Elevator()
{
	delete m_pDispatcherToElevator_consumer;
	delete m_pDispatcherToElevator_producer;
	delete m_pElevatorToIO_consumer;
	delete m_pElevatorToIO_producer;
	delete m_pElevatorDatapool;
	delete m_pElevatorCommands;
}

//

//Initiliazes the default values for the Elevator
void Elevator::UpdateElevatorStatus(ElevatorStatusPtr_t elevatorStatus, int direction, int doorStatus, int floorNumber) const
{
	
		m_pElevatorToIO_consumer->Wait();
		elevatorStatus->direction = direction;
		elevatorStatus->doorStatus = doorStatus;
		elevatorStatus->floorNumber = floorNumber;
		m_pElevatorToIO_producer->Signal();

	/*if(m_elevatorNumber == 1)
		MOVE_CURSOR(10,10);
	else if(m_elevatorNumber == 2)
		MOVE_CURSOR(20,20);

	printf("Updated Elevator Status %d\n", m_elevatorNumber);
	Sleep(1000);*/
}

int Elevator::main()
{

	ElevatorStatusPtr_t	elevatorStatus = (ElevatorStatusPtr_t)(m_pElevatorDatapool->LinkDataPool());
	
	//int floorNumber;
	

	if(m_elevatorNumber == 1)
		UpdateElevatorStatus(elevatorStatus, k_idle, k_closed, 0); // initlialize elevator

	else if(m_elevatorNumber == 2)
		UpdateElevatorStatus(elevatorStatus, k_up, k_open, 9); 
	do{

		Sleep(1500);
		if(elevatorStatus->floorNumber < k_maxFloorNumber && m_elevatorNumber == 1)
			UpdateElevatorStatus(elevatorStatus, elevatorStatus->direction, elevatorStatus->doorStatus, elevatorStatus->floorNumber+1);

		if(elevatorStatus->floorNumber > k_minFloorNumber && m_elevatorNumber == 2)
			UpdateElevatorStatus(elevatorStatus, elevatorStatus->direction, elevatorStatus->doorStatus, elevatorStatus->floorNumber-1);
			//floorNumber = (elevatorStatus->floorNumber)+1;
	
		


	}while(1);
	//delete elevator1Status;
	//delete elevator2Status;
	return 0;
}