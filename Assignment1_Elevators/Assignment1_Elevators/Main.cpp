/*****************************************************
 * EECE 314 Assignment #1
 * Date: October 2013
 * Authors: 
 *	 Kieren Wou	54724117
 *	 Ryan Wong	47307103
 *****************************************************/
#include <iostream>
#include <memory>

#include "..\..\..\rt.h"
#include "Dispatcher.h" //include active classes header files
#include "Elevator.h"
#include "IOProgram.h"


int main()
{
	CMutex	screenMutex("PrintToScreen");
	InterprocessCommTypeNames_t elevator1InterprocessNames,elevator2InterprocessNames;

	elevator1InterprocessNames.dispatcherToElevator_consumer = k_dispatcherToElevator1_consumer;
	elevator1InterprocessNames.dispatcherToElevator_producer = k_dispatcherToElevator1_producer;
	elevator1InterprocessNames.elevatorToIO_consumer = k_elevator1ToIO_consumer;
	elevator1InterprocessNames.elevatorToIO_producer = k_elevator1ToIO_producer;
	elevator1InterprocessNames.dataPool = k_elevator1StatusDataPool;
	elevator1InterprocessNames.elevatorCommands = k_elevator1Commands;

	elevator2InterprocessNames.dispatcherToElevator_consumer = k_dispatcherToElevator2_consumer;
	elevator2InterprocessNames.dispatcherToElevator_producer = k_dispatcherToElevator2_producer;
	elevator2InterprocessNames.elevatorToIO_consumer = k_elevator2ToIO_consumer;
	elevator2InterprocessNames.elevatorToIO_producer = k_elevator2ToIO_producer;
	elevator2InterprocessNames.dataPool = k_elevator2StatusDataPool;
	elevator2InterprocessNames.elevatorCommands = k_elevator2Commands;


	Elevator	elevator1(1,elevator1InterprocessNames);
	Elevator	elevator2(2,elevator2InterprocessNames);


	elevator1.Resume();
	elevator2.Resume();
	Sleep(1000);
	IOProgramPtr_t pIoProgram = std::make_shared<IOProgram>(2);
	Dispatcher dispatcher(pIoProgram,2);

	pIoProgram->Resume();
	dispatcher.Resume();

	pIoProgram->WaitForThread();
	dispatcher.WaitForThread();

	system("PAUSE");
	return 0;
}
