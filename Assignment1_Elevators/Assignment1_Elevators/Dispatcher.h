/*****************************************************
 * EECE 314 Assignment #1
 * Date: October 2013
 * Authors: 
 *	 Kieren Wou	54724117
 *	 Ryan Wong	47307103
 *****************************************************/
#include <iostream>

#include "rt.h"
#include "GlobalVariableDecl.h"
#include "IOProgram.h"
#include "Elevator.h"


#ifndef DISPATCHER_H
#define DISPATCHER_H

class Dispatcher : public ActiveClass
{
public:
	Dispatcher();								
	Dispatcher(
		IOProgramPtr_t pIoProgram, 
		int numberOfElevators); 
	~Dispatcher();											

	int ReadFromIoToDispatcherPipeline(void* args);			//Thread for reading from pipeline3 and send data to elevators through pipelines
	
	int CollectElevatorStatus(void* args);			//Updating elevator local status data pools
	void UpdateElevatorStatus(ElevatorStatus_t elevatorStatus, int elevatorNumber) const; ///for debugging purposes

private:
	int main();

	CDataPool*				m_pElevatorDataPool[k_maximumElevators];	//data pool object pointers to link to elevator status data pools
	CPipe*					m_pElevatorCommands[k_maximumElevators];	//Pipelines to send commands to elevators
	ElevatorStatusPtr_t		m_pElevatorStatus[k_maximumElevators];		//link to the elevator status data pools
	ElevatorStatus_t		m_localElevatorStatus[k_maximumElevators];	//local data structures for elevator statuses
	FloorRequest_t			m_localFloorRequest[k_maximumElevators];	//local data structures for floor requests
	CMutex*					m_screenMutex;				// mutex to allow exclusive updates to the console
	CMutex*					m_getCommandFromIO;			//mutex to lock the data being collected from the IO pipeline
	UserInputData_t			m_userInputData;			//member variable to store data from thread to main function
	FloorRequestVect_t		m_floorRequestVect;			//vector (represents the queue) that holds all floor requests that need servicing
	int						m_numberOfElevators;		//stores number of elevators 
	IOProgramPtr_t			m_pIoProgram;				//Pointer to IOprogram mailbox so that we can post to it
	
	CSemaphore* m_pEntryToQueue;
	CSemaphore* m_pExitFromQueue;
	CSemaphore* m_pQueueEmpty;
	CSemaphore* m_pQueueFull;
	CMutex* m_pQueueMutex;

	bool m_bExit;
	bool m_bIsStartUp;

};
#endif