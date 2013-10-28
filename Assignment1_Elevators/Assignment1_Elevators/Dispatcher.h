#include <iostream>
#include "..\..\..\rt.h"
#include "IOProgram.h"
#include "Elevator.h"

#ifndef DISPATCHER_H
#define DISPATCHER_H

class Dispatcher : public ActiveClass
{
public:
	Dispatcher(); //default constructor
	Dispatcher(IOProgram* pIoProgram); //overloader constructor

	int ReadFromPipeline3(void *args); //Thread for reading from pipeline3 

private:

	IOProgram* m_pIoProgram;
	bool m_exit;
	int main(); 

};
#endif