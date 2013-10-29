/*****************************************************
 * EECE 314 Assignment #1
 * Authors: Kieren Wou & Ryan Wong
 * Date: October 2013
 *****************************************************/
#include <iostream>
#include <memory>

#include "..\..\..\rt.h"




#ifndef ELEVATOR_H
#define ELEVATOR_H

static int const k_open = 1;
static int const k_closed = 0;
static int const k_idle = 0;
static int const k_up = 1;
static int const k_down = 2;
static int const k_maxFloorNumber = 9;
static int const k_minFloorNumber = 0;

static string const k_ioToDispatcherPipeline = "IoToDispatcherPipeline";

static string const k_dispatcherToElevator1_consumer = "DispatcherToElevator1Consumer";
static string const k_dispatcherToElevator1_producer = "DispatcherToElevator1Producer";
static string const k_elevator1ToIO_consumer = "Elevator1ToIOConsumer";
static string const k_elevator1ToIO_producer = "Elevator1ToIOProducer";
static string const k_elevator1StatusDataPool = "Elevator1Status";
static string const k_elevator1Commands = "Elevator1Commands";

static string const k_dispatcherToElevator2_consumer = "DispatcherToElevator2Consumer";
static string const k_dispatcherToElevator2_producer = "DispatcherToElevator2Producer";
static string const k_elevator2ToIO_consumer = "Elevator2ToIOConsumer";
static string const k_elevator2ToIO_producer = "Elevator2ToIOProducer";
static string const k_elevator2StatusDataPool = "Elevator2Status";
static string const k_elevator2Commands = "Elevator2Commands";
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