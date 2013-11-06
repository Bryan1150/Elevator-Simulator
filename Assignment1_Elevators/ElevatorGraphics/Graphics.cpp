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
	MOVE_CURSOR(x+3,y-1);
	TEXT_COLOUR(3);
	printf("LVL %d", (y-80)/(-8));
	TEXT_COLOUR(7);
	for(int i = 0; i < 6; ++i)
	{
		MOVE_CURSOR(x,y+i);
		for(int i = 0; i < 5; ++i)
		{
				printf("%c",219);
		}
		printf("%c",221);
		for(int i = 0; i < 5; ++i)
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
	MOVE_CURSOR(x,y);
		for(int i = 0; i < 2; ++i)
		{
			printf("%c",219);
		}
		printf("%c",221);
		for(int i = 0; i < 6; ++i)
		{
			printf("-"); //overscore
		}
		printf("%c",222);
		for(int i = 0; i < 2; ++i)
		{
			printf("%c",219);
		}

	
	for(int i = 1; i <= 4; ++i)
	{
		MOVE_CURSOR(x,y+i);
		for(int i = 0; i < 2; ++i)
		{
			printf("%c",219);
		}
		printf("%c",221);
		for(int i = 0; i < 6; ++i)
		{
			printf(" "); //overscore
		}
		printf("%c",222);
		for(int i = 0; i < 2; ++i)
		{
			printf("%c",219);
		}
		
	}
	MOVE_CURSOR(x,y+5);
	
		for(int i = 0; i < 2; ++i)
		{
			printf("%c",219);
		}
		printf("%c",221);
		for(int i = 0; i < 6; ++i)
		{
			printf("-");
		}
		printf("%c",222);
		for(int i = 0; i < 2; ++i)
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
	MOVE_CURSOR(x+3,y-1);
	printf("     ");
	for(int i = 0; i < 6; ++i)
	{
		MOVE_CURSOR(x,y+i);
		for(int i = 0; i < 12; ++i)
		{
			printf(" ");
		}

	}
	screenMutex->Signal();
}