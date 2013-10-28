/*****************************************************
 * EECE 314 Assignment #1
 * Authors: Kieren Wou & Ryan Wong
 * Date: October 2013
 *****************************************************/
#include <iostream>
#include <memory>

#include "..\..\..\rt.h"

#define OPEN 2;
#define CLOSED 1;
#define STATIONARY 0;
#define UP	1;
#define DOWN 0;

#ifndef ELEVATOR_H
#define ELEVATOR_H

typedef struct {		

	//status for elevator struct
	int doorStatus;
	int direction;
	int floorNumber;
} ElevatorStatus_t;

typedef ElevatorStatus_t* ElevatorStatusPtr_t;

class Elevator : public ActiveClass
{
public:
	Elevator();		//default constructor
	Elevator(int num); //overloader constructor

private:
	int main();

	int m_elevatorNumber;

};
#endif