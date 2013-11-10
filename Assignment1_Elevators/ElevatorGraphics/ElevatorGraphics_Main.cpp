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
	// store elevator number and convert it into a string for the pipeline
	std::stringstream ss;
	int elevatorId = *(int*)args;
	ss << elevatorId;
	std::string elevatorNumberStr = ss.str();

	CPipe IoToElevatorGraphics_pipeline("IoToElevatorGraphics"+elevatorNumberStr, 1024); // pipeline to receive elevator statuses from IO
	int previousFloorNumber = 0;				// to hold previous floor number for  clearing graphics
	ElevatorStatus_t elevatorStatus;			// struct for holding elevator status for drawing
	bool faultSet = false;

	do{
		IoToElevatorGraphics_pipeline.Read(&elevatorStatus, sizeof(ElevatorStatus_t));

		if( elevatorStatus.bFault)
		{
			graphicsMtx.Wait();
			if(!faultSet)		// show clearing of the queue for floor requests
			{
				MOVE_CURSOR(15+23*elevatorId,0);
				TEXT_COLOUR(15);
				printf("0 1 2 3 4 5 6 7 8 9");
				faultSet = true;
			}
			Display.ClearElevator(20+23*(elevatorId-1),k_heightOfBuilding-5*previousFloorNumber);			// clear previous elevator drawing
			Display.DrawFaultElevator(20+23*(elevatorId-1),k_heightOfBuilding-5*elevatorStatus.floorNumber); // print elevator with fault 
			graphicsMtx.Signal();
		}
		else
		{
			faultSet = false;
			graphicsMtx.Wait();
			Display.ClearElevator(20+23*(elevatorId-1),k_heightOfBuilding-5*previousFloorNumber); // clear previous elevator
			//Display.PrintElevatorStatus(elevatorId, elevatorStatus);
		
			if(elevatorStatus.doorStatus == k_doorOpen) // if the door is open, it has reached a floor request
			{
				Display.OpenElevatorDoor(20+23*(elevatorId-1),k_heightOfBuilding-5*elevatorStatus.floorNumber); // draw elevator with open doors
				if(elevatorStatus.direction == k_directionUp)		// clear up arrows for outside requests
				{
					MOVE_CURSOR(0,k_heightOfBuilding-5*elevatorStatus.floorNumber);
					TEXT_COLOUR(15);
					printf("%c",30);
				}
				else if(elevatorStatus.direction == k_directionDown)   // clear down arrows for outside requests
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
				Display.DrawElevator(20+23*(elevatorId-1),k_heightOfBuilding-5*elevatorStatus.floorNumber); // draw updated elevator
			graphicsMtx.Signal();
			previousFloorNumber = elevatorStatus.floorNumber;	// store current floor number as previous number for clearing the drawing
		}

	}while(1);

	return 0 ;
}

UINT __stdcall GetFloorRequests (void *args)	// thread function 
{	
	
	CPipe IoOutsideRequestsToElevatorGraphics_pipeline("IoOutsideRequestsToElevatorGraphics",1024);// pipeline to receive inside floor requests
	UserInputData_t userInput;		// struct to hold incoming data
	graphicsMtx.Wait();

	// draw floors with arrows and labels
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

		if(userInput.direction == 'E' && userInput.floor == 'E' ) // if end of simulation flag is set, break out of this loop
			break;

		// detect whether an command for UP or DOWN is sent and highlight the corresponding arrows to show this
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

	
	
	CPipe IoInsideRequestsToElevatorGraphics_pipeline("IoInsideRequestsToElevatorGraphics",1024); // pipeline to receive inside requests from IO
	int elevatorId = *(int*)args; // store elevator ID number
	UserInputData_t userInput;	 // struct to store floor requests

	// print values for showing inside requests
	graphicsMtx.Wait();
	for(int i = 0; i < elevatorId; ++i)
	{
		MOVE_CURSOR(15+23*i,0);
		TEXT_COLOUR(15);
		printf("0 1 2 3 4 5 6 7 8 9");
	}
	graphicsMtx.Signal();

	do{
		IoInsideRequestsToElevatorGraphics_pipeline.Read(&userInput, sizeof(UserInputData_t)); // read data from pipeline

		if(userInput.direction == 'E' && userInput.floor == 'E' ) // if end of simulation command is sent, break out of this loop
			break;

		// highlight the number associated with the received floor request
		graphicsMtx.Wait();
		TEXT_COLOUR(14);
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
	CThread *Threads[10]; // array with pointers to threads that update elevator drawings
	CThread getFloorRequests(GetFloorRequests,ACTIVE,NULL); // thread to receive outside floor requests
	CThread getInsideRequests(GetInsideRequests,ACTIVE,&numberOfElevators); // thread to receive inside floor requests
	int elevatorNumberArray[10]; // an array to store elevator IDs
	
	// initialize threads for drawing elevators
	for(int i = 0; i < numberOfElevators; ++i)
	{
		elevatorNumberArray[i] = i+1;
		Threads[i] = new CThread(PrintElevatorGraphics, ACTIVE, &elevatorNumberArray[i]);
	}

	CURSOR_OFF(); // turns cursor off

	
	//wait for threads to terminate
	getFloorRequests.WaitForThread();
	getInsideRequests.WaitForThread();

	// terminate the child threads for end of simulation
	for(int i = 0; i < numberOfElevators; ++i)
	{
		Threads[i]->TerminateThread();
	}

	// delete child pointers for child threads
	for(int i = 0; i < numberOfElevators; ++i)
	{
		delete Threads[i];
	}

	return 0;
}