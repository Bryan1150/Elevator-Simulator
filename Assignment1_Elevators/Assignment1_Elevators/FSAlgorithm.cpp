/*****************************************************
 * EECE 314 Assignment #1
 * Date: October 2013
 * Authors: 
 *	 Kieren Wou	54724117
 *	 Ryan Wong	47307103
 *****************************************************/
#include "FSAlgorithm.h"

void FSAlgorithm::TowardsCallSameDirection(
		FloorRequest_t const& floorReq,
		ElevatorStatus_t& lift)
{
	FigureOfSuitability_t fs;
	fs = (k_numFloorsMinusOne + 2) - abs(floorReq.floorNumber - lift.floorNumber);

	InsertFsIntoMap(fs, floorReq, lift);
}

void FSAlgorithm::TowardsCallDiffDirection(
		FloorRequest_t const& floorReq,
		ElevatorStatus_t& lift)
{
	FigureOfSuitability_t fs;
	fs = (k_numFloorsMinusOne + 1) - abs(floorReq.floorNumber - lift.floorNumber);

	InsertFsIntoMap(fs, floorReq, lift);
}

void FSAlgorithm::AwayFromCall(
		FloorRequest_t const& floorReq,
		ElevatorStatus_t& lift)
{
	FigureOfSuitability_t fs = 1;

	InsertFsIntoMap(fs, floorReq, lift);
}

void FSAlgorithm::InsertFsIntoMap(
	FigureOfSuitability_t const& fs,
	FloorRequest_t const& floorReq,
	ElevatorStatus_t& lift)
{
	auto it = lift.fsToFloorRequestMap.find(fs);
	if(it == lift.fsToFloorRequestMap.end())
	{
		lift.fsToFloorRequestMap.insert(std::make_pair(fs, floorReq));
	}
	else
	{
		if(it->second.elevatorId == INT_MAX) // map has an OUTSIDE request
		{	// INT_MAX is the default
			if(floorReq.elevatorId != INT_MAX) // next entry is an INSIDE request, so overwrite map entry
			{
				lift.fsToFloorRequestMap.erase(it);
				lift.fsToFloorRequestMap.insert(std::make_pair(fs, floorReq));
			}
			else // next entry is *also* an OUTSIDE request, so take the closest floor to the elevator
			{
				if(abs(lift.floorNumber - it->second.floorNumber) > abs(lift.floorNumber - floorReq.floorNumber))
				{
					lift.fsToFloorRequestMap.erase(it);
					lift.fsToFloorRequestMap.insert(std::make_pair(fs, floorReq));
				}
				// else, do nothing if the elevator is closer to the request that is already in the map
			}
		}
		else // map has an INSIDE request
		{
			if(floorReq.elevatorId != INT_MAX) // next entry is an INSIDE request
			{
				if(abs(lift.floorNumber - it->second.floorNumber) > abs(lift.floorNumber - floorReq.floorNumber))
				{
					lift.fsToFloorRequestMap.erase(it);
					lift.fsToFloorRequestMap.insert(std::make_pair(fs, floorReq));
				}
				// else, do nothing if the elevator is closer to the request that is already in the map
			}
			// else, do nothing if next entry is an OUTSIDE request since it ha lower priority
		}
	}
}

// elevatorStatusVect contains DataPool struct from each elevator; passed into each Child Dispatcher Thread
// floorRequestVect contains all requests from IOProgram; need to be deleted by dispatcher once elevator(s) service the requests
ElevatorStatus_t FSAlgorithm::DispatcherFsCalculator(
		ElevatorStatusVect_t& elevatorStatusVect,
		FloorRequestVect_t& floorRequestVect)
{
	// remove a floor request from the queue if it has already been serviced
	for(auto itElevatorStatus = elevatorStatusVect.begin(); 
		itElevatorStatus != elevatorStatusVect.end(); 
		++itElevatorStatus)
	{
		for(auto itDeleteRequest = floorRequestVect.begin();
			itDeleteRequest != floorRequestVect.end();
			++itDeleteRequest)
		{
			if((itDeleteRequest->floorNumber == itElevatorStatus->floorNumber) && (itElevatorStatus->doorStatus == k_doorOpen))
				floorRequestVect.erase(itDeleteRequest);
		}
	}


	for(ElevatorStatusVect_t::iterator itElevatorStatus = elevatorStatusVect.begin();
		itElevatorStatus != elevatorStatusVect.end();
		++itElevatorStatus)
	{
		for(auto itFloorRequest = floorRequestVect.begin();
			itFloorRequest != floorRequestVect.end();
			++itFloorRequest)
		{
			if(itElevatorStatus->direction == k_directionIdle)
			{
				if(itFloorRequest->floorNumber > itElevatorStatus->floorNumber)
				{
					if(itFloorRequest->direction == k_directionUp)
						FSAlgorithm::TowardsCallSameDirection(*itFloorRequest, *itElevatorStatus);
					
					else if(itFloorRequest->direction == k_directionDown)
						FSAlgorithm::TowardsCallDiffDirection(*itFloorRequest, *itElevatorStatus);
				}
				else if(itFloorRequest->floorNumber < itElevatorStatus->floorNumber)
				{
					if(itFloorRequest->direction == k_directionDown)
						FSAlgorithm::TowardsCallSameDirection(*itFloorRequest, *itElevatorStatus);
					
					else if(itFloorRequest->direction == k_directionUp)
						FSAlgorithm::TowardsCallDiffDirection(*itFloorRequest, *itElevatorStatus);
				}
				else if(itFloorRequest->floorNumber == itElevatorStatus->floorNumber)
				{
					FSAlgorithm::TowardsCallSameDirection(*itFloorRequest, *itElevatorStatus);
				}
			}

			else if(itElevatorStatus->direction == itFloorRequest->direction)
			{
				if(itElevatorStatus->direction == k_directionUp)
				{
					if(itFloorRequest->floorNumber >= itElevatorStatus->floorNumber)
						FSAlgorithm::TowardsCallSameDirection(*itFloorRequest, *itElevatorStatus);
					
					else if(itFloorRequest->floorNumber < itElevatorStatus->floorNumber)
						FSAlgorithm::AwayFromCall(*itFloorRequest, *itElevatorStatus);
				}
				else if(itElevatorStatus->direction == k_directionDown)
				{
					if(itFloorRequest->floorNumber <= itElevatorStatus->floorNumber)
						FSAlgorithm::TowardsCallSameDirection(*itFloorRequest, *itElevatorStatus);
					
					else if(itFloorRequest->floorNumber > itElevatorStatus->floorNumber)
						FSAlgorithm::AwayFromCall(*itFloorRequest, *itElevatorStatus);
				}
			}

			else // if request direction and elevator direction are opposites
			{
				if(itElevatorStatus->direction == k_directionUp)
				{
					if(itFloorRequest->floorNumber >= itElevatorStatus->floorNumber)
						FSAlgorithm::TowardsCallDiffDirection(*itFloorRequest, *itElevatorStatus);
					
					else if(itFloorRequest->floorNumber < itElevatorStatus->floorNumber)
						FSAlgorithm::AwayFromCall(*itFloorRequest, *itElevatorStatus);
				}
				else if(itElevatorStatus->direction == k_directionDown)
				{
					if(itFloorRequest->floorNumber <= itElevatorStatus->floorNumber)
						FSAlgorithm::TowardsCallDiffDirection(*itFloorRequest, *itElevatorStatus);
					
					else if(itFloorRequest->floorNumber > itElevatorStatus->floorNumber)
						FSAlgorithm::AwayFromCall(*itFloorRequest, *itElevatorStatus);
				}
			}
		}
	}

	std::vector<int> maxFsVect;
	auto itLift = elevatorStatusVect.begin();
	for(itLift; itLift != elevatorStatusVect.end(); ++itLift)
	{	
		maxFsVect.reserve(elevatorStatusVect.size());
		maxFsVect.push_back(itLift->fsToFloorRequestMap.rbegin()->first);
	}
	auto iter = std::max_element(maxFsVect.begin(), maxFsVect.end());
	
	itLift += std::distance(maxFsVect.begin(), iter);

	return *itLift;
		
}