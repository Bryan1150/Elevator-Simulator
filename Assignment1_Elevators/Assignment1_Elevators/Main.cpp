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
	dispatcher.WaitForThread();
	
	for( int i = 0; i < numberOfElevators; i++)
	{
		delete elevatorVect[i];
		//printf("Deleted elevatorVect %d in Main\n",i+1);
	}

	//FloorRequestVect_t floorRequestVect;
	//FloorRequest_t floorReq1(5,k_directionDown,INT_MAX);
	//FloorRequest_t floorReq2(4,k_directionDown,INT_MAX);
	//FloorRequest_t floorReq3(3,k_directionUp,INT_MAX);
	//FloorRequest_t floorReq4(1,k_directionUp,INT_MAX);

	//floorRequestVect.push_back(floorReq1);
	//floorRequestVect.push_back(floorReq2);
	//floorRequestVect.push_back(floorReq3);
	//floorRequestVect.push_back(floorReq4);

	//ElevatorStatus_t elevStatus1, elevStatus2, elevStatus3, elevStatus4;
	//elevStatus1.direction = k_directionUp;
	//elevStatus1.doorStatus = k_doorOpen;
	//elevStatus1.floorNumber = 1;
	//elevStatus2.direction = k_directionUp;
	//elevStatus2.doorStatus = k_doorOpen;
	//elevStatus2.floorNumber = 2;
	//elevStatus3.direction = k_directionUp;
	//elevStatus3.doorStatus = k_doorOpen;
	//elevStatus3.floorNumber = 3;
	//elevStatus4.direction = k_directionDown;
	//elevStatus4.doorStatus = k_doorOpen;
	//elevStatus4.floorNumber = 5;
	//ElevatorStatusVect_t elevatorStatusVect;
	//elevatorStatusVect.push_back(elevStatus1);
	//elevatorStatusVect.push_back(elevStatus2);
	//elevatorStatusVect.push_back(elevStatus3);
	//elevatorStatusVect.push_back(elevStatus4);

	//FloorRequestVect_t fReqVect = FSAlgorithm::DispatcherFsCalculator(elevatorStatusVect, floorRequestVect);
	
	system("PAUSE");
	return 0;
}
