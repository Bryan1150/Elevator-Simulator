/*****************************************************
 * EECE 314 Assignment #1
 * Authors: Kieren Wou & Ryan Wong
 * Date: October 2013
 *****************************************************/
#include <iostream>

#include "..\..\..\rt.h"

#include "IOProgram.h"
#include "Elevator.h"


static UINT const k_terminateSimulation = 3000;

#ifndef DISPATCHER_H
#define DISPATCHER_H

class Dispatcher : public ActiveClass
{
public:
	Dispatcher(); //default constructor
	Dispatcher(IOProgramPtr_t pIoProgram); //overloader constructor

	int ReadFromPipeline3(void* args); //Thread for reading from pipeline3 
	int WriteToPipeline1(void* args);
	int WriteToPipeline2(void* args);

private:
	int main(); 
	
	IOProgramPtr_t m_pIoProgram;
	bool m_bExit;

};
#endif