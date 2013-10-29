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


int main()
{
	CMutex	screenMutex("PrintToScreen");
	InterprocessCommTypeNames_t elevator1InterprocessNames,elevator2InterprocessNames;

	elevator1InterprocessNames.dispatcherToElevator_consumer = "DispatcherToElevator1Consumer";
	elevator1InterprocessNames.dispatcherToElevator_producer = "DispatcherToElevator1Producer";
	elevator1InterprocessNames.elevatorToIO_consumer = "Elevator1ToIOConsumer";
	elevator1InterprocessNames.elevatorToIO_producer = "Elevator1ToIOProducer";
	elevator1InterprocessNames.dataPool = "Elevator1Status";
	elevator1InterprocessNames.elevatorCommands = "Elevator1Commands";

	elevator2InterprocessNames.dispatcherToElevator_consumer = "DispatcherToElevator2Consumer";
	elevator2InterprocessNames.dispatcherToElevator_producer = "DispatcherToElevator2Producer";
	elevator2InterprocessNames.elevatorToIO_consumer = "Elevator2ToIOConsumer";
	elevator2InterprocessNames.elevatorToIO_producer = "Elevator2ToIOProducer";
	elevator2InterprocessNames.dataPool = "Elevator2Status";
	elevator2InterprocessNames.elevatorCommands = "Elevator2Commands";


	Elevator	elevator1(1,elevator1InterprocessNames);
	Elevator	elevator2(2,elevator2InterprocessNames);


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
