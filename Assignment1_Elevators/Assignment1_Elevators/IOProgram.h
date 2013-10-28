#include <iostream>
#include "..\..\..\rt.h"
#include "Elevator.h"

#ifndef IOPROGRAM_H
#define IOPROGRAM_H

struct	UserCommands{
	
	char Direction;
	char Floor;
};

class IOProgram : public ActiveClass
{
public:
	IOProgram(); //default constructor

	int ReadFromMailbox(void* args);

private:

	bool m_exit;
	int main();

};
#endif