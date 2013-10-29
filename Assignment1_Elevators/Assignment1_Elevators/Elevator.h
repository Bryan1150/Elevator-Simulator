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
static int const k_stationary = 0;
static int const k_up = 1;
static int const k_down = 2;
static int const k_maxFloorNumber = 9;
static int const k_minFloorNumber = 0;
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

	void UpdateElevatorStatus(ElevatorStatusPtr_t elevatorStatus, int direction, int doorStatus, int floorNumber) const;

private:
	int main();

	int m_elevatorNumber;

};
#endif