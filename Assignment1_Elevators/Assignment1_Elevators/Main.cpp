#include <iostream>
#include "..\..\..\rt.h"

#include "Dispatcher.h" //include active classes header files
#include "Elevator.h"
#include "IOProgram.h"

int main()
{
	IOProgram ioProgram;
	Dispatcher dispatcher(&ioProgram);

	ioProgram.Resume();
	dispatcher.Resume();

	ioProgram.WaitForThread();
	dispatcher.WaitForThread();

	system("PAUSE");
	return 0;
}
