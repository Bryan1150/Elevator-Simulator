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

	std::vector<Elevator*> elevatorVect;

	for( int i = 0; i < numberOfElevators; i++)
	{
		
			Elevator* elevator= new Elevator(i+1);
			elevatorVect.push_back(elevator);
			elevatorVect[i]->Resume();
		//add delete in for the pointers in the vectors //add waitfor thread at the end
	}

	Sleep(1000);
	IOProgramPtr_t pIoProgram = std::make_shared<IOProgram>(numberOfElevators);
	Dispatcher dispatcher(pIoProgram,numberOfElevators);

	pIoProgram->ClearLines(3);
	pIoProgram->Resume();
	dispatcher.Resume();

	pIoProgram->WaitForThread();
	dispatcher.WaitForThread();
	/*for( int i = 0; i < numberOfElevators; i++)
	{
		elevatorVect[i]->WaitForThread();
	}*/
	for( int i = 0; i < numberOfElevators; i++)
	{
		delete elevatorVect[i];
		//printf("Deleted elevatorVect %d in Main\n",i+1);
	}
	system("PAUSE");
	return 0;
}
