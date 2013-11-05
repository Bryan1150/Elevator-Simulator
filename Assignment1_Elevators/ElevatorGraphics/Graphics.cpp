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
	MOVE_CURSOR(x+1,y);
	printf("___________");
	for(int i = 1; i < 6; ++i)
	{
		MOVE_CURSOR(x,y+i);
		printf("|     |     |");
	}
	MOVE_CURSOR(x,y+6);
	printf("|_____|_____|");
	screenMutex->Signal();
}
void Graphics::OpenElevatorDoor(int x, int y)
{
	screenMutex->Wait();
	MOVE_CURSOR(x+1,y);
	printf("___________");
	for(int i = 1; i < 6; ++i)
	{
		MOVE_CURSOR(x,y+i);
		printf("| |       | |");
	}
	MOVE_CURSOR(x,y+6);
	printf("|_|_______|_|");
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
	MOVE_CURSOR(x+1,y);
	printf("           ");
	for(int i = 1; i < 6; ++i)
	{
		MOVE_CURSOR(x,y+i);
		printf("             ");
	}
	MOVE_CURSOR(x,y+6);
	printf("              ");
	screenMutex->Signal();
}