/*****************************************************
 * EECE 314 Assignment #1
 * Authors: Kieren Wou & Ryan Wong
 * Date: October 2013
 *****************************************************/
#include <iostream>
#include <memory>

#include "..\..\..\rt.h"

#include "Dispatcher.h" //include active classes header files
#include "Elevator.h"
#include "IOProgram.h"

#define _CRT_SECURE_NO_WARNINGS	

int main()
{
	CMutex	screenMutex("PrintToScreen");
	Elevator	elevator1(1);
	Elevator	elevator2(2);

	elevator1.Resume();
	elevator2.Resume();
	Sleep(1000);
	IOProgramPtr_t pIoProgram = std::make_shared<IOProgram>();
	Dispatcher dispatcher(pIoProgram);

	pIoProgram->Resume();
	dispatcher.Resume();

	pIoProgram->WaitForThread();
	dispatcher.WaitForThread();

	system("PAUSE");
	return 0;
}
