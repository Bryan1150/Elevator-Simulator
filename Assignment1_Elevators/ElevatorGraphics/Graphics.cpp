#include "Graphics.h"


Graphics::Graphics()
{
	
	screenMutex = new CMutex("Screen Mutex");
}
Graphics::~Graphics()
{
	delete screenMutex;
}


void Graphics::DrawElevator(int x, int y)
{
	screenMutex->Wait();
	MOVE_CURSOR(x+1,y-1);
	TEXT_COLOUR(3);
	printf("LVL %d", (y-k_heightOfBuilding)/(-5));
	TEXT_COLOUR(7);
	for(int i = 0; i < 4; ++i)
	{
		MOVE_CURSOR(x,y+i);
		for(int i = 0; i < 3; ++i)
		{
			printf("%c",219);
		}
		printf("%c",221);
		for(int i = 0; i < 3; ++i)
		{
			printf("%c",219);
		}
		printf("%c",221);

	}
	TEXT_COLOUR(15,0); //set font back to white and background to black
	screenMutex->Signal();
}
void Graphics::OpenElevatorDoor(int x, int y)
{
	screenMutex->Wait();
	MOVE_CURSOR(x+1,y-1);
	TEXT_COLOUR(3);
	printf("LVL %d", (y-k_heightOfBuilding)/(-5));
	TEXT_COLOUR(7);
	MOVE_CURSOR(x,y);
		for(int i = 0; i < 1; ++i)
		{
			printf("%c",219);
		}
		printf("%c",221);
		for(int i = 0; i < 4; ++i)
		{
			printf("-"); //overscore
		}
		printf("%c",222);
		for(int i = 0; i < 1; ++i)
		{
			printf("%c",219);
		}

	
	for(int i = 1; i <= 2; ++i)
	{
		MOVE_CURSOR(x,y+i);
		for(int i = 0; i < 1; ++i)
		{
			printf("%c",219);
		}
		printf("%c",221);
		for(int i = 0; i < 4; ++i)
		{
			printf(" "); //overscore
		}
		printf("%c",222);
		for(int i = 0; i < 1; ++i)
		{
			printf("%c",219);
		}
		
	}
	MOVE_CURSOR(x,y+3);
	
		for(int i = 0; i < 1; ++i)
		{
			printf("%c",219);
		}
		printf("%c",221);
		for(int i = 0; i < 4; ++i)
		{
			printf("-");
		}
		printf("%c",222);
		for(int i = 0; i < 1; ++i)
		{
			printf("%c",219);
		}
	screenMutex->Signal();
}
void Graphics::CloseElevatorDoor(int x, int y)
{
	screenMutex->Wait();
	MOVE_CURSOR(x,y);

	screenMutex->Signal();
}

void Graphics::ClearElevator(int x, int y)
{
	screenMutex->Wait();
	MOVE_CURSOR(x+1,y-1);
	printf("     ");
	for(int i = 0; i < 4; ++i)
	{
		MOVE_CURSOR(x,y+i);
		for(int i = 0; i < 10; ++i)
		{
			printf(" ");
		}

	}
	screenMutex->Signal();
}

void Graphics::PrintElevatorStatus(int elevatorId, ElevatorStatus_t const& ElevatorStatus)
{
	screenMutex->Wait();
	MOVE_CURSOR(15+23*(elevatorId-1),0);
	printf("Elevator %d", elevatorId);
	MOVE_CURSOR(15+23*(elevatorId-1),1);
	printf("Floor Number: %d", ElevatorStatus.floorNumber);

	MOVE_CURSOR(15+23*(elevatorId-1),2);
	printf("Direction: ");
	if( ElevatorStatus.direction == 0)
		 printf("Idle");
	else if( ElevatorStatus.direction == 1)
		printf("Up  ");
	else if( ElevatorStatus.direction == 2)
		printf("Down");

	MOVE_CURSOR(15+23*(elevatorId-1),3);
	printf("Door Status: ");
	if( ElevatorStatus.doorStatus == 0)
		 printf("Closed");
	else if( ElevatorStatus.doorStatus == 1)
		printf("Open  ");
	
	screenMutex->Signal();
}