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
#include "rt.h"

#pragma once


class FSAlgorithm {
public:
	static void TowardsCallSameDirection(
		FloorRequest_t const& floorReq,
		ElevatorStatus_t& Lift);

	static void TowardsCallDiffDirection(
		FloorRequest_t const& floorReq,
		ElevatorStatus_t& lift);

	static void AwayFromCall(
		FloorRequest_t const& floorReq,
		ElevatorStatus_t& lift);

	static void InsideRequest(
		FloorRequest_t const& floorReq,
		ElevatorStatus_t& lift);

	static void InsertFsIntoMap(
		FigureOfSuitability_t const& fs,
		FloorRequest_t const& floorReq,
		ElevatorStatus_t& lift);
	
	static FloorRequestVect_t DispatcherFsCalculator(
		ElevatorStatusVect_t& elevatorStatusVect,
		FloorRequestVect_t& floorRequestVect,
		bool bIsStartUp);
};