#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <string.h>
#include <map>
#include "Graphics.h"
#include "..\Assignment1_Elevators\GlobalVariableDecl.h"
#include "..\..\..\rt.h"

Graphics Display;
CMutex graphicsMtx("Mutex for Drawing");
//
//typedef struct {
//	int floorNumber;
//	int direction;
//	int elevatorId;
//
//} FloorRequest_t;
//typedef int FigureOfSuitability_t;
//typedef std::map<FigureOfSuitability_t, FloorRequest_t> FsToFloorRequestMap_t;
//typedef struct {		
//
//	//status for elevator struct
//	int doorStatus;
//	int direction;
//	int floorNumber;
//	FsToFloorRequestMap_t fsToFloorRequestMap;
//
//} ElevatorStatus_t;

UINT __stdcall PrintElevatorGraphics (void *args)	// thread function 
{	
	std::stringstream ss;
	int elevatorId = *(int*)args;
	ss << elevatorId;
	std::string elevatorNumberStr = ss.str();
	CPipe IoToElevatorGraphics_pipeline("IoToElevatorGraphics"+elevatorNumberStr, 1024);
	int previousFloorNumber = 0;
	ElevatorStatus_t elevatorStatus;
	do{
		IoToElevatorGraphics_pipeline.Read(&elevatorStatus, sizeof(ElevatorStatus_t));
		graphicsMtx.Wait();
		Display.ClearElevator(20+23*(elevatorId-1),64-6*previousFloorNumber);
		Display.DrawElevator(20+23*(elevatorId-1),64-6*elevatorStatus.floorNumber);
		Display.PrintElevatorStatus(elevatorId, elevatorStatus);
		graphicsMtx.Signal();
		previousFloorNumber = elevatorStatus.floorNumber;	
		

	}while(1);

	return 0 ;
}

UINT __stdcall GetFloorRequests (void *args)	// thread function 
{	
	//Need pipeline
	graphicsMtx.Wait();
	for(int i = 0; i <= 9; ++i)
	{
		MOVE_CURSOR(0,64-6*i-2);
		printf("____________");
		MOVE_CURSOR(0,64-6*i-1);
		printf("Floor %d", i);
		MOVE_CURSOR(0,64-6*i);
		printf("%c",30);
		MOVE_CURSOR(0,64-6*i+1);
		printf("%c",31);
	}
	graphicsMtx.Signal();

	do{
	

	}while(1);

	return 0 ;
}

UINT __stdcall GetInsideRequests (void *args)	// thread function 
{	

	
	//Need pipeline
	int elevatorId = *(int*)args;
	graphicsMtx.Wait();
	for(int i = 0; i < elevatorId; ++i)
	{
		MOVE_CURSOR(15+23*i,6);
		printf("0 1 2 3 4 5 6 7 8 9");
	}
	graphicsMtx.Signal();

	do{
	

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
	
	RECT r;
	HWND console = GetConsoleWindow();
	GetWindowRect(console, &r); //stores the console's current dimensions
	MoveWindow(console, r.left, r.top, 800, 600, TRUE);

	for(int i = 0; i < numberOfElevators; ++i)
	{
		xArray[i] = i+1;
		Threads[i] = new CThread(PrintElevatorGraphics, ACTIVE, &xArray[i]);
	}

	CURSOR_OFF();

	for(int i = 0; i < numberOfElevators; ++i)
	{
		Threads[i]->WaitForThread();
	}

	for(int i = 0; i < numberOfElevators; ++i)
	{
		delete Threads[i];
	}

	return 0;
}