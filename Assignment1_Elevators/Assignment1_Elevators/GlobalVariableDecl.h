/*****************************************************
 * EECE 314 Assignment #1
 * Date: October 2013
 * Authors: 
 *	 Kieren Wou	54724117
 *	 Ryan Wong	47307103
 *****************************************************/
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <sstream>

#pragma once

static int const k_open = 1;
static int const k_closed = 0;
static int const k_idle = 0;
static int const k_up = 1;
static int const k_down = 2;
static int const k_maxFloorNumber = 9;
static int const k_minFloorNumber = 0;

static int const k_terminateSimulation = 3000;

static std::string const k_ioToDispatcherPipeline = "IoToDispatcherPipeline";


typedef struct {
	char direction;
	char floor;
} UserInputData_t;

struct FloorRequest_t{
	
	std::string fReqId;
	int floorNumber;
	int direction;
	int elevatorId;
	bool bInsideRequest;

	FloorRequest_t()
		: bInsideRequest(false)
		, floorNumber(INT_MAX)
		, direction(INT_MAX)
		, elevatorId(INT_MAX)
		, fReqId("")
	{}
	
	FloorRequest_t(int floor, int dir)
		: bInsideRequest(false)
		, floorNumber(floor)
		, direction(dir)
		, elevatorId(INT_MAX)
	{
		if(dir == k_up)
			fReqId = "U";
		else if(dir == k_down)
			fReqId = "D";

		std::stringstream ss;
		ss << floor;
		fReqId += ss.str(); // ex. "U1","D9", etc.		
	}

	FloorRequest_t(int floor, int dir, int elevId)
		: bInsideRequest(elevatorId != INT_MAX ? true : false)
		, floorNumber(floor)
		, direction(dir)
		, elevatorId(elevId)
	{
		if(dir == k_up)
			fReqId = "U";
		else if(dir == k_down)
			fReqId = "D";

		std::stringstream ss;
		ss << floorNumber;
		fReqId += ss.str(); // ex. "U1","D9", etc.		
	}

	bool operator>(FloorRequest_t const& other) const
	{ return fReqId > other.fReqId; }

	bool operator>=(FloorRequest_t const& other) const
	{ return fReqId >= other.fReqId; }
	
	bool operator<(FloorRequest_t const& other) const
	{ return fReqId < other.fReqId; }
	
	bool operator<=(FloorRequest_t const& other) const
	{ return fReqId <= other.fReqId; }

	bool operator==(FloorRequest_t const& other) const
	{ return fReqId == other.fReqId; }
};

typedef int FigureOfSuitability_t;
typedef std::map<FigureOfSuitability_t, FloorRequest_t> FsToFloorRequestMap_t;

typedef struct {		

	//status for elevator struct
	int doorStatus;
	int direction;
	int floorNumber;
	FsToFloorRequestMap_t fsToFloorRequestMap;
} ElevatorStatus_t;

typedef std::vector<ElevatorStatus_t> ElevatorStatusVect_t;
typedef std::vector<FloorRequest_t> FloorRequestVect_t;

