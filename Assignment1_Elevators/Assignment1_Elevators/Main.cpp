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
	*for( int i = 0; i < numberOfElevators; i++)
	{
		elevatorVect[i]->WaitForThread();
	}*/
	for( int i = 0; i < numberOfElevators; i++)
	{
		delete elevatorVect[i];
		//printf("Deleted elevatorVect %d in Main\n",i+1);
	}

	//FloorRequestVect_t floorRequestVect;
	//FloorRequest_t floorReq1;
	//floorReq1.direction = k_directionUp;
	//floorReq1.elevatorId = INT_MAX;
	//floorReq1.floorNumber = 3;
	//floorRequestVect.push_back(floorReq1);

	//ElevatorStatus_t elevStatus1, elevStatus2;
	//elevStatus1.direction = k_directionUp;
	//elevStatus1.doorStatus = k_doorOpen;
	//elevStatus1.floorNumber = 1;
	//elevStatus2.direction = k_directionUp;
	//elevStatus2.doorStatus = k_doorOpen;
	//elevStatus2.floorNumber = 2;
	//ElevatorStatusVect_t elevatorStatusVect;
	//elevatorStatusVect.push_back(elevStatus1);
	//elevatorStatusVect.push_back(elevStatus2);

	//ElevatorStatus_t elevStatusResult = FSAlgorithm::DispatcherFsCalculator(elevatorStatusVect, floorRequestVect);
	//
	system("PAUSE");
	return 0;
}
