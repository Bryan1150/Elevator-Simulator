/*****************************************************
 * EECE 314 Assignment #1
 * Authors: Kieren Wou & Ryan Wong
 * Date: October 2013
 *****************************************************/
#include "Elevator.h"


Elevator::Elevator() 
	: m_elevatorNumber(0) // FIXME kwou: change default value
{} 

Elevator::Elevator(int num) 
	: m_elevatorNumber(num)
{}


int Elevator::main()
{

	CDataPool elevator1_datapool("ElevatorStatus1", sizeof(ElevatorStatus_t));
	CDataPool elevator2_datapool("ElevatorStatus2", sizeof(ElevatorStatus_t));
	
	CPipe		ECommands1("Elevator1Commands", 1024);
	CPipe		ECommands2("Elevator2Commands", 1024);

	//ElevatorStatusPtr_t	elevator1Status = (ElevatorStatusPtr_t)(elevator1_datapool.LinkDataPool());
	//ElevatorStatusPtr_t	elevator2Status = (ElevatorStatusPtr_t)(elevator2_datapool.LinkDataPool());


	//delete elevator1Status;
	//delete elevator2Status;
	return 0;
}