#include "IOProgram.h"

struct ElevatorStatus{   // struct for elevator status

};

IOProgram::IOProgram(){}




int IOProgram::main()
{
	CDataPool	dp1("ElevatorStatus1", sizeof(struct ElevatorStatus));
	CDataPool	dp2("ElevatorStatus2", sizeof(struct ElevatorStatus));
	CPipe		DCommands("DispatcherCommands", 1024);

	struct ElevatorStatus		*EStatus1 = (struct ElevatorStatus*)(dp1.LinkDataPool());
	struct ElevatorStatus		*EStatus2 = (struct ElevatorStatus*)(dp2.LinkDataPool());

	delete EStatus1;
	delete EStatus2;
	return 0;
}