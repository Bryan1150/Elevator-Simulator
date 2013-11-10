/*****************************************************
 * EECE 314 Assignment #1
 * Date: October 2013
 * Authors: 
 *	 Kieren Wou	54724117
 *	 Ryan Wong	47307103
 *****************************************************/
#include <iostream>
#include <memory>
#include <vector>

#include "rt.h"
#include "Dispatcher.h" //include active classes header files
#include "Elevator.h"
#include "IOProgram.h"
#include "GlobalVariableDecl.h"


int main()
{
	CMutex screenMutex("PrintToScreen");
	int numberOfElevators;

	std::cout << "Enter number of elevators: ";
	std::cin >> numberOfElevators;
	int i;
	std::vector<ElevatorSharedPtr_t> elevatorVect;

	for( int i = 0; i < numberOfElevators; i++)
	{
		ElevatorSharedPtr_t pElevator = std::make_shared<Elevator>(i+1);
		elevatorVect.push_back(pElevator);
		elevatorVect[i]->Resume();
	}

	Sleep(1000);
	IOProgramPtr_t pIoProgram = std::make_shared<IOProgram>(numberOfElevators);
	Dispatcher dispatcher(pIoProgram,numberOfElevators);

	pIoProgram->ClearLines(3);
	pIoProgram->Resume();
	dispatcher.Resume();

	dispatcher.WaitForThread();
	pIoProgram->WaitForThread();
	//dispatcher.TerminateThread();
	
	for( int i = 0; i < numberOfElevators; i++)
	{
		elevatorVect[i]->EndChildThread();
		elevatorVect[i]->TerminateThread();	
	}
	printf("Deleted child elevator threads in Main.cpp .\n") ;
	return 0;
}
