/*****************************************************
 * EECE 314 Assignment #1
 * Date: October 2013
 * Authors: 
 *	 Kieren Wou	54724117
 *	 Ryan Wong	47307103
 *****************************************************/
#include <complex>
#include <map>
#include <vector>
#include <algorithm>

#pragma once

typedef struct {
	int floor;
	int direction;
	int elevatorId;

} FloorRequest_t;

typedef int FigureOfSuitability_t;
typedef std::map<FigureOfSuitability_t, FloorRequest_t> FsToFloorRequestMap_t;

typedef struct {
	int floor;
	int door;
	int direction;
	FsToFloorRequestMap_t fsToFloorRequestMap;
	std::string dataPoolId;

} ElevatorStatus_t;

typedef std::vector<ElevatorStatus_t> ElevatorStatusVect_t;
typedef std::vector<FloorRequest_t> FloorRequestVect_t;

static int const k_doorOpen = 0;
static int const k_doorClosed = 1;
static int const k_directionDown = 0;
static int const k_directionUp = 1;
static int const k_directionIdle = 2;

class FSAlgorithm {
public:
	static int const k_numFloorsMinusOne = 4;

	static void TowardsCallSameDirection(
		FloorRequest_t const& floorReq,
		ElevatorStatus_t& Lift);

	static void TowardsCallDiffDirection(
		FloorRequest_t const& floorReq,
		ElevatorStatus_t& lift);

	static void AwayFromCall(
		FloorRequest_t const& floorReq,
		ElevatorStatus_t& lift);

	static void InsertFsIntoMap(
		FigureOfSuitability_t const& fs,
		FloorRequest_t const& floorReq,
		ElevatorStatus_t& lift);
	
	static ElevatorStatus_t DispatcherFsCalculator(
		ElevatorStatusVect_t& elevatorStatusVect,
		FloorRequestVect_t& floorRequestVect);
};