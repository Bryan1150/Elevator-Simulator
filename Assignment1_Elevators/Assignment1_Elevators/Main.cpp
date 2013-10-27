#include <iostream>
#include "..\..\..\rt.h"

#include "Dispatcher.h" //include active classes header files
#include "Elevator.h"
#include "IOProgram.h"

int main()
{
	IOProgram io;
	
	io.Resume();
	Sleep(1000);
	io.Post(2000);
	




	Sleep(5000);
	io.WaitForThread();


	system("PAUSE");
	return 0;
}
