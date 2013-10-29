/*****************************************************
 * EECE 314 Assignment #1
 * Authors: Kieren Wou & Ryan Wong
 * Date: October 2013
 *****************************************************/
#include "Elevator.h"


Elevator::Elevator() 
	: m_elevatorNumber(0) // FIXME kwou: change default value
{} 

Elevator::Elevator(int num, InterprocessCommTypeNames_t interprocessCommTypeNames) 
	: m_elevatorNumber(num)
{
	m_dispatcherToElevator_consumer = new CSemaphore(interprocessCommTypeNames.dispatcherToElevator_consumer,1,1);
	m_dispatcherToElevator_producer = new CSemaphore(interprocessCommTypeNames.dispatcherToElevator_consumer,0,1);
	m_elevatorToIO_consumer = new CSemaphore(interprocessCommTypeNames.elevatorToIO_consumer,1,1);
	m_elevatorToIO_producer = new CSemaphore(interprocessCommTypeNames.elevatorToIO_producer,0,1);
	m_elevatorDatapool = new CDataPool(interprocessCommTypeNames.dataPool,sizeof(ElevatorStatus_t));
	m_elevatorCommands = new CPipe(interprocessCommTypeNames.elevatorCommands, 1024);
	
}

//Initiliazes the default values for the Elevator
void Elevator::UpdateElevatorStatus(ElevatorStatusPtr_t elevatorStatus, int direction, int doorStatus, int floorNumber) const
{
	/*CSemaphore dispatcherToElevator1_consumer("DispatcherToElevator1Consumer",1,1);
	CSemaphore dispatcherToElevator1_producer("DispatcherToElevator1Producer",0,1);
	CSemaphore elevator1ToIO_consumer("Elevator1ToIOConsumer",1,1);
	CSemaphore elevator1ToIO_producer("Elevator1ToIOProducer",0,1);

	CSemaphore elevator2ToIO_consumer("Elevator2ToIOConsumer",1,1);
	CSemaphore elevator2ToIO_producer("Elevator2ToIOProducer",0,1);
	CSemaphore dispatcherToElevator2_consumer("DispatcherToElevator2Consumer",1,1);
	CSemaphore dispatcherToElevator2_producer("DispatcherToElevator2Producer",0,1);*/
	
	/*if(m_elevatorNumber == 1)
		elevator1ToIO_consumer.Wait();
	else if(m_elevatorNumber ==2 )
		elevator2ToIO_consumer.Wait();*/
		m_elevatorToIO_consumer->Wait();
		elevatorStatus->direction = direction;
		elevatorStatus->doorStatus = doorStatus;
		elevatorStatus->floorNumber = floorNumber;
		m_elevatorToIO_producer->Signal();
	/*if(m_elevatorNumber == 1)
		elevator1ToIO_producer.Signal();
	else if(m_elevatorNumber ==2 )
		elevator2ToIO_producer.Signal();*/

	/*if(m_elevatorNumber == 1)
		MOVE_CURSOR(10,10);
	else if(m_elevatorNumber == 2)
		MOVE_CURSOR(20,20);

	printf("Updated Elevator Status %d\n", m_elevatorNumber);
	Sleep(1000);*/
}

int Elevator::main()
{

	/*CDataPool elevator1_datapool("Elevator1Status", sizeof(ElevatorStatus_t));
	CDataPool elevator2_datapool("Elevator2Status", sizeof(ElevatorStatus_t));*/
	
	/*CSemaphore dispatcherToElevator1_consumer("DispatcherToElevator1Consumer",1,1);
	CSemaphore dispatcherToElevator1_producer("DispatcherToElevator1Producer",1,1);
	CSemaphore elevator1ToIO_consumer("Elevator1ToIOConsumer",1,1);
	CSemaphore elevator1ToIO_producer("Elevator1ToIOProducer",1,1);

	CSemaphore elevator2ToIO_consumer("Elevator2ToIOConsumer",1,1);
	CSemaphore elevator2ToIO_producer("Elevator2ToIOProducer",1,1);
	CSemaphore dispatcherToElevator2_consumer("DispatcherToElevator2Consumer",1,1);
	CSemaphore dispatcherToElevator2_producer("DispatcherToElevator2Producer",1,1);*/
/*
	CPipe		ECommands1("Elevator1Commands", 1024);
	CPipe		ECommands2("Elevator2Commands", 1024);*/
	ElevatorStatusPtr_t	elevatorStatus = (ElevatorStatusPtr_t)(m_elevatorDatapool->LinkDataPool());
	/*ElevatorStatusPtr_t	elevator2Status = (ElevatorStatusPtr_t)(elevator2_datapool.LinkDataPool());*/
	/*ElevatorStatusPtr_t	currentElevator;*/
	int floorNumber;

	/*if(m_elevatorNumber == 1)
		currentElevator = elevator1Status;
	else if(m_elevatorNumber == 2)
		currentElevator = elevator2Status;
	else
		printf("Error in initializing elevator statuses");*/

	UpdateElevatorStatus(elevatorStatus, k_idle, k_closed, 0);
	do{

		Sleep(1500);
		if(elevatorStatus->floorNumber < k_maxFloorNumber)
			floorNumber = (elevatorStatus->floorNumber)+1;
	
		UpdateElevatorStatus(elevatorStatus, k_idle, k_closed, floorNumber);


	}while(1);
	//delete elevator1Status;
	//delete elevator2Status;
	return 0;
}