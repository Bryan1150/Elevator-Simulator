#include <iostream>
#include "..\..\..\rt.h"

#ifndef IOPROGRAM_H
#define IOPROGRAM_H

class IOProgram : public ActiveClass
{
public:
	IOProgram(); //default constructor

private:
	int main();

};
#endif