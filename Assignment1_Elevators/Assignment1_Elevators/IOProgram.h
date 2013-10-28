/*****************************************************
 * EECE 314 Assignment #1
 * Authors: Kieren Wou & Ryan Wong
 * Date: October 2013
 *****************************************************/
#include <iostream>
#include <memory>

#include "..\..\..\rt.h"

#include "Elevator.h"


#ifndef IOPROGRAM_H
#define IOPROGRAM_H

class IOProgram;
typedef std::shared_ptr<IOProgram> IOProgramPtr_t;

typedef struct {
	char direction;
	char floor;
} UserInputData_t;

class IOProgram : public ActiveClass
{
public:
	IOProgram(); //default constructor

	int ReadFromMailbox(void* args);
	int IsValidCommand(UserInputData_t userInput) const;

private:

	bool m_exit;
	int main();

};
#endif