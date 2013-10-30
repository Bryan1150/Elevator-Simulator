/*****************************************************
 * EECE 314 Assignment #1
 * Date: October 2013
 * Authors: 
 *	 Kieren Wou	54724117
 *	 Ryan Wong	47307103
 *****************************************************/
#include <iostream>

#include "..\..\..\rt.h"
#include "GlobalVariableDecl.h"
#include "IOProgram.h"
#include "Elevator.h"


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
	ElevatorStatusPtr_t	m_pElevator1Status, m_pElevator2Status;
	bool m_bExit;

};
#endif