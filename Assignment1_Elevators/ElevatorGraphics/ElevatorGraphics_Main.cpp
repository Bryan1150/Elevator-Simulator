#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <string.h>
#include <map>
#include "Graphics.h"
#include "..\..\..\rt.h"

Graphics Display;
CMutex graphicsMtx("Mutex for Drawing");

typedef struct {
	int floorNumber;
	int direction;
	int elevatorId;

} FloorRequest_t;
typedef int FigureOfSuitability_t;
typedef std::map<FigureOfSuitability_t, FloorRequest_t> FsToFloorRequestMap_t;
typedef struct {		

	//status for elevator struct
	int doorStatus;
	int direction;
	int floorNumber;
	FsToFloorRequestMap_t fsToFloorRequestMap;

} ElevatorStatus_t;

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
		Display.ClearElevator(25*elevatorId,36-4*previousFloorNumber);
		Display.DrawElevator(25*elevatorId,36-4*elevatorStatus.floorNumber);
		graphicsMtx.Signal();
		previousFloorNumber = elevatorStatus.floorNumber;


	}while(1);

	return 0 ;
}

int main(int argc, char* argv[])
{
	int numberOfElevators = atoi(argv[1]);
	CThread *Threads[10];
	//Threads = (CThread*)malloc(numberOfElevators*sizeof(CThread));
	int xArray[10];

	for(int i = 0; i < numberOfElevators; ++i)
	{
		xArray[i] = i+1;
		Threads[i] = new CThread(PrintElevatorGraphics, ACTIVE, &xArray[i]);
	}


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