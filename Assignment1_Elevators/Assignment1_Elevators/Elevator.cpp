/*****************************************************
 * EECE 314 Assignment #1
 * Date: October 2013
 * Authors: 
 *	 Kieren Wou	54724117
 *	 Ryan Wong	47307103
 *****************************************************/
#include <sstream>

#include "Elevator.h"
#include "IOProgram.h"

Elevator::Elevator() 
	: m_elevatorNumber(0) // FIXME kwou: change default value
	, m_elevatorStatus(ElevatorStatus_t())
{
	m_pDispatcherToElevator_consumer = new CSemaphore("dispatcherToElevator_consumer",1,1);
	m_pDispatcherToElevator_producer = new CSemaphore("dispatcherToElevator_consumer",0,1);
	m_pChildToMainElev_consumer = new CSemaphore("ChildToMainElevConsumer",1,1);
	m_pChildToMainElev_producer = new CSemaphore("ChildToMainElevProducer",0,1);
	m_pElevatorToIO_consumer = new CSemaphore("elevatorToIO_consumer",1,1);
	m_pElevatorToIO_producer = new CSemaphore("elevatorToIO_producer",0,1);

	m_pElevatorDatapool = new CDataPool("dataPool",sizeof(ElevatorStatus_t));
	m_pElevatorCommands = new CPipe("elevatorCommands", 1024);
	m_pDispatcherFloorRequest = new CMutex("DispatcherFloorRequest");
	m_pScreenMutex = new CMutex("PrintToScreen");
} 

Elevator::Elevator(int num) 
	: m_elevatorNumber(num)
	, m_elevatorStatus(ElevatorStatus_t())
{
	std::stringstream ss;
	std::string elevatorNumber;
	ss << m_elevatorNumber;
	elevatorNumber = ss.str();

	m_pDispatcherToElevator_consumer = new CSemaphore("DispatcherToElevator"+elevatorNumber+"Consumer",1,1);
	m_pDispatcherToElevator_producer = new CSemaphore("DispatcherToElevator"+elevatorNumber+"Producer",0,1);
	m_pElevatorToIO_consumer = new CSemaphore("Elevator"+elevatorNumber+"ToIOConsumer",1,1);
	m_pElevatorToIO_producer = new CSemaphore("Elevator"+elevatorNumber+"ToIOProducer",0,1);
	m_pChildToMainElev_consumer = new CSemaphore("ChildToMainElevConsumer",1,1);
	m_pChildToMainElev_producer = new CSemaphore("ChildtoMainElevProducer",0,1);

	m_pElevatorDatapool = new CDataPool("Elevator"+elevatorNumber+"Status",sizeof(ElevatorStatus_t));
	m_pElevatorCommands = new CPipe("Elevator"+elevatorNumber+"Commands", 1024);
	m_pDispatcherFloorRequest = new CMutex("DispatcherFloorRequest");
	m_pScreenMutex = new CMutex("PrintToScreen");

	m_pdispatcherToElevatorPipelineThread = new ClassThread<Elevator>(this, &Elevator::ReadCommandsFromPipeline,ACTIVE, NULL);
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
	delete m_pdispatcherToElevatorPipelineThread;
}

//
int  Elevator::ReadCommandsFromPipeline(void* args)
{
	std::stringstream ss;
	ss << m_elevatorNumber;
	std::string elevatorNumber = ss.str();
	
	CPipe elevatorCommands("Elevator"+elevatorNumber+"Commands", 1024);
	
	FloorRequest_t floorRequest;
	UserInputData_t userInput;

	do{
		if(elevatorCommands.TestForData() >= sizeof(FloorRequest_t))
		{
			elevatorCommands.Read(&floorRequest,sizeof(FloorRequest_t));

			m_pChildToMainElev_consumer->Wait();
			
			m_floorReqFromDispatcher = floorRequest;
			OutputDebugString("Elevator Child finished reading FR from DispatcherToElevator Pipeline and sending to Elevator Main\n");
			
			m_pChildToMainElev_producer->Signal();
		}

	} while(1);
	return 0;
}

void Elevator::EndChildThread()
{
	m_pdispatcherToElevatorPipelineThread->TerminateThread();
}

int Elevator::main()
{
	ElevatorStatusPtr_t	pElevatorStatusDP = (ElevatorStatusPtr_t)(m_pElevatorDatapool->LinkDataPool());

	FloorRequest_t floorRequest(0, k_directionUp);
	FloorRequest_t lastRequest(0, k_directionUp);
		
	bool bIsStartUp = true;

	// very first time only
	m_pDispatcherToElevator_consumer->Wait();
	m_pElevatorToIO_consumer->Wait(); 
	
	OutputDebugString("Elevator Main writing new ElevatorStatus to DP\n");
	
	pElevatorStatusDP->direction = m_elevatorStatus.direction;
	pElevatorStatusDP->doorStatus = m_elevatorStatus.doorStatus;
	pElevatorStatusDP->floorNumber = m_elevatorStatus.floorNumber;
	
	m_pDispatcherToElevator_producer->Signal();
	m_pElevatorToIO_producer-> Signal();

	do { 
		m_pChildToMainElev_producer->Wait();

		floorRequest = m_floorReqFromDispatcher;
		OutputDebugString("Elevator Main has received FR from DispatcherToElevatorPipeline\n");
		
		m_pChildToMainElev_consumer->Signal();
		
		// We send a default FR to an elevator with the following condition below, when
		// it doesn't need to move. It will stay in place until the highest FS of a new FR matches this elevator
		if(floorRequest.fReqId == "Idle")
		{
			m_elevatorStatus.doorStatus = k_doorOpen;
			m_pScreenMutex->Wait();
			MOVE_CURSOR(0,30);
			std::cout << "                                                   ";
			MOVE_CURSOR(0,30);
			std::cout << "Elevator is idle at floor " << m_elevatorStatus.floorNumber << std::endl;
			m_pScreenMutex->Signal();
		}

		else if(m_elevatorStatus.floorNumber == floorRequest.floorNumber)
		{
			m_elevatorStatus.direction = floorRequest.direction;
			m_elevatorStatus.doorStatus = k_doorOpen;
			m_pScreenMutex->Wait();
			MOVE_CURSOR(0,30);
			std::cout << "                                                   ";
			MOVE_CURSOR(0,30);
			std::cout << "Elevator has reached its FR at floor " << floorRequest.floorNumber << std::endl;
			m_pScreenMutex->Signal();
			Sleep(2000);
		}
		else
		{
			m_elevatorStatus.doorStatus = k_doorClosed;

			m_elevatorStatus.floorNumber - floorRequest.floorNumber > 0 ? m_elevatorStatus.direction = k_directionDown : m_elevatorStatus.direction = k_directionUp;
			if(m_elevatorStatus.direction == k_directionDown)
				m_elevatorStatus.floorNumber--;
			else if(m_elevatorStatus.direction == k_directionUp)
				m_elevatorStatus.floorNumber++;

			m_pScreenMutex->Wait();
			MOVE_CURSOR(0,30);
			std::cout << "                                                   ";
			MOVE_CURSOR(0,30);
			std::cout << "Elevator is at floor " << m_elevatorStatus.floorNumber << std::endl;
			m_pScreenMutex->Signal();
		}
		
		m_pDispatcherToElevator_consumer->Wait();
		m_pElevatorToIO_consumer->Wait(); 

		pElevatorStatusDP->direction = m_elevatorStatus.direction;
		pElevatorStatusDP->doorStatus = m_elevatorStatus.doorStatus;
		pElevatorStatusDP->floorNumber = m_elevatorStatus.floorNumber;

		OutputDebugString("Elevator Main writing new ElevatorStatus to DP\n");

		m_pDispatcherToElevator_producer->Signal();
		m_pElevatorToIO_producer-> Signal();

		Sleep(500);

	} while(1);


	
	return 0;
}