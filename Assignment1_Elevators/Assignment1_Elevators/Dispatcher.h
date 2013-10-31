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
	Dispatcher();													//default constructor
	Dispatcher(IOProgramPtr_t pIoProgram, int numberOfElevators); //overloader constructor
	~Dispatcher();												//destructor


	int ReadFromIoToDispatcherPipeline(void* args);				//Thread for reading from pipeline3 and send data to elevators through pipelines
	int DispatcherToElevator(void* args);			//Thread to send commands to elevators (currently not being used
	
	int CollectElevatorStatus(void* args);			//Updating elevator local status data pools
	void UpdateElevatorStatus(ElevatorStatus_t elevatorStatus, int elevatorNumber) const; ///for debugging purposes

private:
	CDataPool*			m_pElevatorDataPool[100];	//data pool object pointers to link to elevator status data pools
	CPipe*				m_pElevatorCommands[100];	//Pipelines to send commands to elevators
	ElevatorStatusPtr_t m_pElevatorStatus[100];		//link to the elevator status data pools
	ElevatorStatus_t	m_localElevatorStatus[100];	//local data structures for elevator statuses
	FloorRequest_t		m_localFloorRequest[100];	//local data structures for floor requests
	CMutex*				m_screenMutex;				// mutex to allow exclusive updates to the console
	CMutex*				m_getCommandFromIO;			//mutex to lock the data being collected from the IO pipeline

	int main(); 
	int					m_numberOfElevators;		//stores number of elevators 
	IOProgramPtr_t		m_pIoProgram;				//Pointer to IOprogram mailbox so that we can post to it
	
	
	bool m_bExit;

};
#endif