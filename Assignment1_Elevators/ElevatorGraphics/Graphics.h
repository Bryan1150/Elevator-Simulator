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

	void DrawElevator(int x, int y);
	void OpenElevatorDoor(int x, int y);
	void CloseElevatorDoor(int x, int y);
	void ClearElevator(int x, int y);
	void PrintElevatorStatus(int elevatorId, ElevatorStatus_t const& ElevatorStatus);

private:
	CMutex* screenMutex;

};
#endif