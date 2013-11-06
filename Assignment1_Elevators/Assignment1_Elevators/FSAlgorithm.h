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

#include "GlobalVariableDecl.h"
#include "..\..\..\rt.h"

#pragma once



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
	
	static FloorRequestVect_t DispatcherFsCalculator(
		ElevatorStatusVect_t& elevatorStatusVect,
		FloorRequestVect_t& floorRequestVect);
};