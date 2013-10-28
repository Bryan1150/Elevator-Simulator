#include "Elevator.h"




Elevator::Elevator() : elevator_number(0){} /*************change default value***************/

Elevator::Elevator(int num) : elevator_number(num){}


int Elevator::main()
{

	CDataPool	dp1("ElevatorStatus1", sizeof(struct ElevatorStatus));
	CDataPool	dp2("ElevatorStatus2", sizeof(struct ElevatorStatus));
	CPipe		ECommands1("Elevator1Commands", 1024);
	CPipe		ECommands2("Elevator2Commands", 1024);

	//struct ElevatorStatus		*EStatus1 = (struct ElevatorStatus*)(dp1.LinkDataPool());
	//struct ElevatorStatus		*EStatus2 = (struct ElevatorStatus*)(dp2.LinkDataPool());

	//delete EStatus1;
	//delete EStatus2;
	return 0;
}