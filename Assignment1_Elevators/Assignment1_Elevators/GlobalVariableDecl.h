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

static int const k_doorOpen = 1;
static int const k_doorClosed = 0;
static int const k_directionDown = 2;
static int const k_directionUp = 1;
static int const k_directionIdle = 0;

static int const k_open = 1;
static int const k_closed = 0;
static int const k_idle = 0;
static int const k_up = 1;
static int const k_down = 2;
static int const k_maxFloorNumber = 9;
static int const k_minFloorNumber = 0;

static int const k_heightOfBuilding = 47;
static int const k_numFloorsMinusOne = 9;

static std::string const k_idleFReqIdStr("Idle"); 

static int const k_terminateSimulation = 3000;

static std::string const k_ioToDispatcherPipeline = "IoToDispatcherPipeline";


struct UserInputData_t {
	
	char direction;
	char floor;

	UserInputData_t()
	{}

	UserInputData_t(char dir, char fl)
		: direction(dir)
		, floor(fl)
	{}
};

struct FloorRequest_t{
	
	std::string fReqId;
	int floorNumber;
	int direction;
	int elevatorId;
	bool bInsideRequest;
	bool bDuplicateFloor;

	FloorRequest_t()
		: bInsideRequest(false)
		, floorNumber(INT_MAX)
		, direction(INT_MAX)
		, elevatorId(INT_MAX)
		, fReqId("Idle")
		, bDuplicateFloor(false)
	{}
	
	FloorRequest_t(int floor, int dir)
		: bInsideRequest(false)
		, floorNumber(floor)
		, direction(dir)
		, elevatorId(INT_MAX)
		, bDuplicateFloor(false)
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
		: floorNumber(floor)
		, direction(dir) // if dir == INT_MAX, must set dir of given elevId
		, elevatorId(elevId)
		, bInsideRequest(elevId != INT_MAX ? true : false)
		, bDuplicateFloor(false) 
	{
		if(dir == k_up)
			fReqId = "U";
		else if(dir == k_down)
			fReqId = "D";

		std::stringstream ss;
		ss << floorNumber;
		fReqId += ss.str(); // ex. "U1","D9", etc.		
	}

		FloorRequest_t(int floor, int dir, std::string frId)
		: bInsideRequest(false)
		, floorNumber(floor)
		, direction(dir)
		, fReqId(frId)
		, elevatorId(INT_MAX)
		, bDuplicateFloor(false)
	{}

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

	bool operator!=(FloorRequest_t const& other) const
	{ return fReqId != other.fReqId; }

};

typedef int FigureOfSuitability_t;
typedef std::map<FigureOfSuitability_t, FloorRequest_t> FsToFloorRequestMap_t;

struct ElevatorStatus_t {
	
	int doorStatus;
	int direction;
	int floorNumber;
	bool bFault;
	FsToFloorRequestMap_t fsToFloorRequestMap;

	ElevatorStatus_t()
		: doorStatus(k_open)
		, direction(k_directionUp)
		, floorNumber(0)
		, bFault(false)
	{}

	ElevatorStatus_t& operator=(ElevatorStatus_t const& other)
	{
		doorStatus = other.doorStatus;
		direction = other.direction;
		floorNumber = other.floorNumber;
		bFault = other.bFault;
		if(!other.fsToFloorRequestMap.empty())
			fsToFloorRequestMap = other.fsToFloorRequestMap;

		return *this;
	}

};

typedef std::vector<ElevatorStatus_t> ElevatorStatusVect_t;
typedef std::vector<FloorRequest_t> FloorRequestVect_t;

class Elevator;
typedef std::shared_ptr<Elevator> ElevatorSharedPtr_t;

