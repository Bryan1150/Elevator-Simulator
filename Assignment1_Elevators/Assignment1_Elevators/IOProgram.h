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
#include "Elevator.h"


#ifndef IOPROGRAM_H
#define IOPROGRAM_H

class IOProgram;
typedef std::shared_ptr<IOProgram> IOProgramPtr_t;



class IOProgram : public ActiveClass
{
public:
	IOProgram(); //default constructor

	IOProgram(int numberOfElevators, std::string path);
	~IOProgram();
	
	//int ReadFromMailbox(void* args);							//Not being used currently
	bool IsValidCommand(UserInputData_t userInput) const;		//checks to ensure command entered is valid
	void ClearLines(int lines) const;							//clear lines in console

	int CollectElevatorStatus(void* args);						//gather elevator status from the data pools
	//void UpdateElevatorStatus(ElevatorStatus_t elevatorStatus, int elevatorNumber) const;	//Print elevator statuses on the console
	
private:

	CDataPool* m_pElevatorDataPool[10];					//data pool object pointers to link to elevator status data pools
	ElevatorStatusPtr_t m_pElevatorStatus[10];				//link to the elvator status data pools
	ElevatorStatus_t m_localElevatorStatus[10];			//local data structures for elevator statuses
	CMutex*	m_screenMutex;									//mutext to allow exclusive updates to the console
	bool m_exit;											//termination flag
	int m_numberOfElevators;								//stores number of elevators							
	int main();
	std::string m_path;

};
#endif