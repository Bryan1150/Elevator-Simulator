/*****************************************************
 * EECE 314 Assignment #1
 * Date: October 2013
 * Authors: 
 *	 Kieren Wou	54724117
 *	 Ryan Wong	47307103
 *****************************************************/
#include <iostream>
#include "..\Assignment1_Elevators\GlobalVariableDecl.h"
#include "..\Assignment1_Elevators\rt.h"



#ifndef GRAPHICS_H
#define GRAPHICS_H

class Graphics
{
public:
	Graphics();
	~Graphics();

	void DrawElevator(int x, int y);			// draws elevator with closed doors
	void OpenElevatorDoor(int x, int y);		// draws elevator with open doors
	void DrawFaultElevator(int x, int y);		// draws elevator with faults
	void ClearElevator(int x, int y);			// clears a previously drawn elevator
	//void PrintElevatorStatus(int elevatorId, ElevatorStatus_t const& ElevatorStatus);

private:
	CMutex* screenMutex;			//mutex for protecting what is being drawn on the screen
	bool m_fault;					// boolean used to draw flashing colours when there is a fault
};
#endif