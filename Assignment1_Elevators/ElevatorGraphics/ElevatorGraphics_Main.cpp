#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <string.h>
#include <map>
#include "Graphics.h"
#include "..\Assignment1_Elevators\GlobalVariableDecl.h"
#include "..\Assignment1_Elevators\rt.h"

Graphics Display;
CMutex graphicsMtx("Mutex for Drawing");


UINT __stdcall PrintElevatorGraphics (void *args)	// thread function 
{	
	std::stringstream ss;
	int elevatorId = *(int*)args;
	ss << elevatorId;
	std::string elevatorNumberStr = ss.str();
	CPipe IoToElevatorGraphics_pipeline("IoToElevatorGraphics"+elevatorNumberStr, 1024);
	int previousFloorNumber = 0;
	bool bexit = false;
	ElevatorStatus_t elevatorStatus;
	do{
		IoToElevatorGraphics_pipeline.Read(&elevatorStatus, sizeof(ElevatorStatus_t));

		/*if(elevatorStatus.direction == 1000 && elevatorStatus.doorStatus == 2000)
			break;*/

	/*	if(!bexit)
		{*/
			graphicsMtx.Wait();
			Display.ClearElevator(20+23*(elevatorId-1),k_heightOfBuilding-5*previousFloorNumber);
			//Display.PrintElevatorStatus(elevatorId, elevatorStatus);
		
			if(elevatorStatus.doorStatus == k_doorOpen)
			{
				Display.OpenElevatorDoor(20+23*(elevatorId-1),k_heightOfBuilding-5*elevatorStatus.floorNumber);
				if(elevatorStatus.direction == k_directionUp)
				{
					MOVE_CURSOR(0,k_heightOfBuilding-5*elevatorStatus.floorNumber);
					TEXT_COLOUR(15);
					printf("%c",30);
				}
				else if(elevatorStatus.direction == k_directionDown)
				{
					if( elevatorStatus.floorNumber !=9 )
						MOVE_CURSOR(0,k_heightOfBuilding-5*elevatorStatus.floorNumber+1);
					else if( elevatorStatus.floorNumber == 9)
						MOVE_CURSOR(0,k_heightOfBuilding-5*elevatorStatus.floorNumber);
					TEXT_COLOUR(15);
					printf("%c",31);
				}
				MOVE_CURSOR(15+23*(elevatorId-1)+2*elevatorStatus.floorNumber,0); //clear inside requests
				printf("%d",elevatorStatus.floorNumber);
			}
			else 
				Display.DrawElevator(20+23*(elevatorId-1),k_heightOfBuilding-5*elevatorStatus.floorNumber);
			graphicsMtx.Signal();
			previousFloorNumber = elevatorStatus.floorNumber;	
		/*}*/

	}while(1);

	return 0 ;
}

UINT __stdcall GetFloorRequests (void *args)	// thread function 
{	
	//Need pipeline
	CPipe IoOutsideRequestsToElevatorGraphics_pipeline("IoOutsideRequestsToElevatorGraphics",1024);/*******************************/
	UserInputData_t userInput;		
	graphicsMtx.Wait();
	MOVE_CURSOR(0,k_heightOfBuilding+3);
	TEXT_COLOUR(15);
	printf("____________");
	for(int i = 0; i <= 9; ++i)
	{
		if( i != 9)	
		{
			MOVE_CURSOR(0,k_heightOfBuilding-5*i-2);
			printf("____________");
		}
		MOVE_CURSOR(0,k_heightOfBuilding-5*i-1);
		printf("Floor %d", i);
		if( i != 9)
		{
			MOVE_CURSOR(0,k_heightOfBuilding-5*i);
			printf("%c",30);
		}
		if( i != 0)
		{
			if( i!=9 )
				MOVE_CURSOR(0,k_heightOfBuilding-5*i+1);
			else if( i == 9)
				MOVE_CURSOR(0,k_heightOfBuilding-5*i);
			printf("%c",31);
		}
	}

	graphicsMtx.Signal();

	do{
		IoOutsideRequestsToElevatorGraphics_pipeline.Read(&userInput, sizeof(UserInputData_t));

		if(userInput.direction == 'E' && userInput.floor == 'E' )
			break;

		if(userInput.direction == 'U')
		{
			graphicsMtx.Wait();
			MOVE_CURSOR(0,k_heightOfBuilding-5*(userInput.floor-'0'));
			TEXT_COLOUR(10);
			printf("%c",30);
			TEXT_COLOUR(15);
			graphicsMtx.Signal();
		}
		else if(userInput.direction == 'D')
		{
			graphicsMtx.Wait();
			if( userInput.floor-'0' !=9 )
				MOVE_CURSOR(0,k_heightOfBuilding-5*(userInput.floor-'0')+1);
			else if( userInput.floor-'0' == 9)
				MOVE_CURSOR(0,k_heightOfBuilding-5*(userInput.floor-'0'));
			TEXT_COLOUR(12);
			printf("%c",31);
			TEXT_COLOUR(15);
			graphicsMtx.Signal();
		}
	}while(1);

	return 0 ;
}

UINT __stdcall GetInsideRequests (void *args)	// thread function 
{	

	
	//Need pipeline
	CPipe IoInsideRequestsToElevatorGraphics_pipeline("IoInsideRequestsToElevatorGraphics",1024);/*******************************/
	int elevatorId = *(int*)args;
	UserInputData_t userInput;	

	graphicsMtx.Wait();
	for(int i = 0; i < elevatorId; ++i)
	{
		MOVE_CURSOR(15+23*i,0);
		TEXT_COLOUR(15);
		printf("0 1 2 3 4 5 6 7 8 9");
	}
	graphicsMtx.Signal();

	do{
		IoInsideRequestsToElevatorGraphics_pipeline.Read(&userInput, sizeof(UserInputData_t));

		if(userInput.direction == 'E' && userInput.floor == 'E' )
			break;

		graphicsMtx.Wait();
		TEXT_COLOUR(9);
		MOVE_CURSOR(15+23*(userInput.direction-'0'-1)+2*(userInput.floor-'0'),0);
		printf("%c",userInput.floor);
		TEXT_COLOUR(15);
		graphicsMtx.Signal();

	}while(1);

	return 0 ;
}



int main(int argc, char* argv[])
{
	int numberOfElevators = atoi(argv[1]);
	CThread *Threads[10];
	CThread getFloorRequests(GetFloorRequests,ACTIVE,NULL);
	CThread getInsideRequests(GetInsideRequests,ACTIVE,&numberOfElevators);
	//Threads = (CThread*)malloc(numberOfElevators*sizeof(CThread));
	int xArray[10];
	
	//RECT r;
	//HWND console = GetConsoleWindow();
	//GetWindowRect(console, &r); //stores the console's current dimensions
	//MoveWindow(console, r.left, r.top, 800, 600, TRUE);

	for(int i = 0; i < numberOfElevators; ++i)
	{
		xArray[i] = i+1;
		Threads[i] = new CThread(PrintElevatorGraphics, ACTIVE, &xArray[i]);
	}

	CURSOR_OFF();


	getFloorRequests.WaitForThread();
	getInsideRequests.WaitForThread();
	for(int i = 0; i < numberOfElevators; ++i)
	{
		Threads[i]->TerminateThread();
	}

	for(int i = 0; i < numberOfElevators; ++i)
	{
		delete Threads[i];
	}

	return 0;
}