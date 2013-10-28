#include <iostream>
#include "..\..\..\rt.h"

#ifndef ELEVATOR_H
#define ELEVATOR_H

struct ElevatorStatus{		//status for elevator struct

};

class Elevator : public ActiveClass
{
public:
	Elevator();		//default constructor
	Elevator(int num); //overloader constructor

private:

	int elevator_number;
	int main();
};
#endif