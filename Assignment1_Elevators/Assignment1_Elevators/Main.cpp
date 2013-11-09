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

#include "..\..\..\rt.h"
#include "Dispatcher.h" //include active classes header files
#include "Elevator.h"
#include "IOProgram.h"


int main()
{
	CMutex	screenMutex("PrintToScreen");
	int numberOfElevators;

	std::cout << "Enter number of elevators: ";
	std::cin >> numberOfElevators;
	int i;
	std::vector<Elevator*> elevatorVect;

	for( int i = 0; i < numberOfElevators; i++)
	{
		
			Elevator* elevator= new Elevator(i+1);
			elevatorVect.push_back(elevator);
			elevatorVect[i]->Resume();
		// FIXME add delete in for the pointers in the vectors; add WaitForThread() at the end
	}

	Sleep(1000);
	IOProgramPtr_t pIoProgram = std::make_shared<IOProgram>(numberOfElevators);
	Dispatcher dispatcher(pIoProgram,numberOfElevators);

	pIoProgram->ClearLines(3);
	pIoProgram->Resume();
	dispatcher.Resume();

	pIoProgram->WaitForThread();
	//dispatcher.TerminateThread();
	
	for( int i = 0; i < numberOfElevators; i++)
	{
		elevatorVect[i]->TerminateThread();
		delete elevatorVect[i];
		//printf("Deleted elevatorVect %d in Main\n",i+1);
	}

	system("PAUSE");
	return 0;
}
