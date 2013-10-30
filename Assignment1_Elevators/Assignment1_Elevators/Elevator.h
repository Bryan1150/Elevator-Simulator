/*****************************************************
 * EECE 314 Assignment #1
 * Date: October 2013
 * Authors: 
 *	 Kieren Wou	54724117
 *	 Ryan Wong	47307103
 *****************************************************/
#include <iostream>
#include <memory>

#include "..\..\..\rt.h"
#include "GlobalVariableDecl.h"


#ifndef ELEVATOR_H
#define ELEVATOR_H

typedef struct {		

	//status for elevator struct
	int doorStatus;
	int direction;
	int floorNumber;
} ElevatorStatus_t;

typedef struct {		

	//string names for interprocess communication
	string dispatcherToElevator_consumer;
	string dispatcherToElevator_producer;
	string elevatorToIO_consumer;
	string elevatorToIO_producer;
	string dataPool;
	string elevatorCommands;
} InterprocessCommTypeNames_t;

typedef ElevatorStatus_t* ElevatorStatusPtr_t;

class Elevator : public ActiveClass
{
public:
	Elevator();		//default constructor
	Elevator(int num,InterprocessCommTypeNames_t interprocessCommTypeNames); //overloader constructor
	~Elevator();	//deconstructor
	void UpdateElevatorStatus(ElevatorStatusPtr_t elevatorStatus, int direction, int doorStatus, int floorNumber) const;
	void OpenElevatorDoors(ElevatorStatusPtr_t elevatorStatus) const;
	void CloseElevatorDoors(ElevatorStatusPtr_t elevatorStatus) const;
	int  ReadCommandsFromPipeline(void* args);
private:
	int main();

	CSemaphore* m_pDispatcherToElevator_consumer;
	CSemaphore* m_pDispatcherToElevator_producer;
	CSemaphore* m_pElevatorToIO_consumer;
	CSemaphore* m_pElevatorToIO_producer;
	CDataPool*  m_pElevatorDatapool;
	CPipe*		m_pElevatorCommands;

	int m_elevatorNumber;

};
#endif