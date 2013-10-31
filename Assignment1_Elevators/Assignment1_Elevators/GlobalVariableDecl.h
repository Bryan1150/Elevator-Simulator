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

static std::string const k_dispatcherToElevator1_consumer = "DispatcherToElevator1Consumer";
static std::string const k_dispatcherToElevator1_producer = "DispatcherToElevator1Producer";
static std::string const k_elevator1ToIO_consumer = "Elevator1ToIOConsumer";
static std::string const k_elevator1ToIO_producer = "Elevator1ToIOProducer";
static std::string const k_elevator1StatusDataPool = "Elevator1Status";
static std::string const k_elevator1Commands = "Elevator1Commands";

static std::string const k_dispatcherToElevator2_consumer = "DispatcherToElevator2Consumer";
static std::string const k_dispatcherToElevator2_producer = "DispatcherToElevator2Producer";
static std::string const k_elevator2ToIO_consumer = "Elevator2ToIOConsumer";
static std::string const k_elevator2ToIO_producer = "Elevator2ToIOProducer";
static std::string const k_elevator2StatusDataPool = "Elevator2Status";
static std::string const k_elevator2Commands = "Elevator2Commands";

typedef struct {
	char direction;
	char floor;
} UserInputData_t;