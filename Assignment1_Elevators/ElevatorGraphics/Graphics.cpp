/*****************************************************
 * EECE 314 Assignment #1
 * Date: October 2013
 * Authors: 
 *	 Kieren Wou	54724117
 *	 Ryan Wong	47307103
 *****************************************************/
#include "Graphics.h"


Graphics::Graphics()
{
	screenMutex = new CMutex("Screen Mutex"); // protect drawing on the console
	m_fault = 0;
}
Graphics::~Graphics()
{
	delete screenMutex;
}

// Draws an elevator with closed doors
void Graphics::DrawElevator(int x, int y)
{
	screenMutex->Wait();
	MOVE_CURSOR(x+1,y-1);
	// Display current floor number
	TEXT_COLOUR(11);
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

// Used for drawing an elevator with open doors
void Graphics::OpenElevatorDoor(int x, int y)
{
	screenMutex->Wait();
	MOVE_CURSOR(x+1,y-1);

	//Display the current elevator level
	TEXT_COLOUR(11);
	printf("LVL %d", (y-k_heightOfBuilding)/(-5));
	TEXT_COLOUR(7);
	MOVE_CURSOR(x,y);
		for(int i = 0; i < 1; ++i) // draws top row of the elevator
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

	
	for(int i = 1; i <= 2; ++i) // draws middle rows of the elevator
	{
		MOVE_CURSOR(x,y+i);
		for(int i = 0; i < 1; ++i)
		{
			printf("%c",219);
		}
		printf("%c",221);
		for(int i = 0; i < 4; ++i)
		{
			printf(" "); 
		}
		printf("%c",222);
		for(int i = 0; i < 1; ++i)
		{
			printf("%c",219);
		}
		
	}
	MOVE_CURSOR(x,y+3);
	
		for(int i = 0; i < 1; ++i) // draws bottom row of the elevator
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
		TEXT_COLOUR(15,0); //set font back to white and background to black
	screenMutex->Signal();
}

// Used for clearing a previously drawn elevator
void Graphics::ClearElevator(int x, int y)
{
	screenMutex->Wait();
	MOVE_CURSOR(x+1,y-1);
	printf("     ");
	for(int i = 0; i < 5; ++i)
	{
		MOVE_CURSOR(x,y+i);
		for(int i = 0; i < 10; ++i)
		{
			printf(" ");
		}

	}
	screenMutex->Signal();
}

// Draws an elevator that has a fault
void Graphics::DrawFaultElevator(int x, int y)
{
	screenMutex->Wait();
	MOVE_CURSOR(x+1,y-1);
	
	// display the current level of the elevator in alternating colours to indicate its fault
	!m_fault ? TEXT_COLOUR(14) : TEXT_COLOUR(12);
	printf("LVL %d", (y-k_heightOfBuilding)/(-5));
	TEXT_COLOUR(7,0);
	MOVE_CURSOR(x,y);
		for(int i = 0; i < 1; ++i) //Draw top row for elevator
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

	
	for(int i = 1; i <= 2; ++i) // print middle rows of the elevator
	{
		MOVE_CURSOR(x,y+i);
		for(int i = 0; i < 1; ++i)
		{
			printf("%c",219);
		}
		printf("%c",221);
		for(int i = 0; i < 4; ++i)
		{
			printf(" "); 
		}
		printf("%c",222);
		for(int i = 0; i < 1; ++i)
		{
			printf("%c",219);
		}
		
	}
	MOVE_CURSOR(x,y+3); 
	
		for(int i = 0; i < 1; ++i) // print bottom row of the elevator
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

	MOVE_CURSOR(x+1,y+4);

	// flash the word "FAULT" underneath the elevator
	!m_fault ? TEXT_COLOUR(12) : TEXT_COLOUR(14);
	printf("FAULT");
	m_fault = (!m_fault) ? true : false;
	TEXT_COLOUR(15,0);
	screenMutex->Signal();
}

