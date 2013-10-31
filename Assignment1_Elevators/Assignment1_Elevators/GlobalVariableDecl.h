/*****************************************************
 * EECE 314 Assignment #1
 * Date: October 2013
 * Authors: 
 *	 Kieren Wou	54724117
 *	 Ryan Wong	47307103
 *****************************************************/
#include <iostream>
#include <string>

#pragma once

static int const k_open = 1;
static int const k_closed = 0;
static int const k_idle = 0;
static int const k_up = 1;
static int const k_down = 2;
static int const k_maxFloorNumber = 9;
static int const k_minFloorNumber = 0;

static UINT const k_terminateSimulation = 3000;

static std::string const k_ioToDispatcherPipeline = "IoToDispatcherPipeline";


typedef struct {
	char direction;
	char floor;
} UserInputData_t;