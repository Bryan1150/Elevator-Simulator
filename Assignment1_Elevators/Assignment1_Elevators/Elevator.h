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
#include "FSAlgorithm.h"

#ifndef ELEVATOR_H
#define ELEVATOR_H



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
	Elevator(int num); //overloader constructor
	~Elevator();	//deconstructor
	void UpdateElevatorStatus(ElevatorStatusPtr_t elevatorStatus, int direction, int doorStatus, int floorNumber) const;
	void OpenElevatorDoors(ElevatorStatusPtr_t elevatorStatus) const;
	void CloseElevatorDoors(ElevatorStatusPtr_t elevatorStatus) const;
	int  ReadCommandsFromPipeline(void* args);
	void ChangeDoorStatus(ElevatorStatusPtr_t elevatorStatus, int doorStatus) const;
	void ChangeFloorNumber(ElevatorStatusPtr_t elevatorStatus, int direction) const;
	void SetElevatorDirection(ElevatorStatusPtr_t elevatorStatus, int direction) const;

private:
	int main();

	CSemaphore* m_pDispatcherToElevator_consumer;
	CSemaphore* m_pDispatcherToElevator_producer;
	CSemaphore* m_pElevatorToIO_consumer;
	CSemaphore* m_pElevatorToIO_producer;
	CDataPool*  m_pElevatorDatapool;
	CPipe*		m_pElevatorCommands;
	CMutex*		m_pScreenMutex;
	CMutex*      m_pDispatcherFloorRequest; //Mutex for floor requests

	FloorRequest_t m_floorReqFromDispatcher;
	ElevatorStatus_t m_elevatorStatus;
	int m_elevatorNumber;

};
#endif