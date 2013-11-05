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

	std::map<std::string/*fReqId*/, std::pair<FigureOfSuitability_t,int/*elevatorId*/>> floorReqToFsMap;
	std::vector<int> reorderQueue;
	auto itLift = elevatorStatusVect.begin();
	for(itLift; itLift != elevatorStatusVect.end(); ++itLift)
	{	
		for(auto itFs = itLift->fsToFloorRequestMap.rbegin();
			itFs != itLift->fsToFloorRequestMap.rend();
			--itFs)
		{
			auto itMap = floorReqToFsMap.find(itFs->second.fReqId/*floorRequest*/);
			if(itMap == floorReqToFsMap.end())
			{
				int elevatorId = std::distance(elevatorStatusVect.begin(), itLift); // NOTE: elevatorId is the index beginning from 0
				floorReqToFsMap.insert(std::make_pair(itFs->second.fReqId, std::make_pair(itFs->first, elevatorId)));
				break;
			}
			else if(itFs->first > itMap->second.first) // floorRequest already in map, so if this next one has a larger FS, then replace the old one
			{	
				int elevatorRemovalId = itMap->second.second;
				
				int elevatorId = std::distance(elevatorStatusVect.begin(), itLift);

				// erase then add new FR with the higher FS value
				floorReqToFsMap.erase(itMap);
				floorReqToFsMap.insert(
					std::make_pair(itLift->fsToFloorRequestMap.rbegin()->second.fReqId,
					std::make_pair(itLift->fsToFloorRequestMap.rbegin()->first, elevatorId)));
				
				// go through the remaining FR/FS for this elevator to find the next highest that is not in floorReqToFsMap
				for(auto itRemoval = --elevatorStatusVect[elevatorRemovalId].fsToFloorRequestMap.rbegin();
					itRemoval != elevatorStatusVect[elevatorRemovalId].fsToFloorRequestMap.rend();
					--itRemoval)
				{
					if(floorReqToFsMap.find(itRemoval->second.fReqId) == floorReqToFsMap.end()) // if FR not yet in map, then insert
					{
						floorReqToFsMap.insert(
							std::make_pair(itRemoval->second.fReqId,
							std::make_pair(itRemoval->first, elevatorRemovalId)));
						
						break;
					}
				}
				
				break;
			}
			// we don't break from loop if a FR still needs to be added to the map for the current elevator
		}
	}

	// Now that we have the FR with the highest FS linked to each elevator, we can tell each elevator where it needs to go next.
	// The floorReqToFsMap will contain n FRs (n <= number of elevators) with a mapped pair containing an "elevatorId". Sometimes,
	// there might not be a FR for a certain elevator, so we send it a blank FR, telling it to remain in the same spot.
	// Elevator statuses from the DPs to the dispatcher are stored in order of "elevatorId".
	// FIXME: add a final vector containing the FR for each elevator in order; return this vector instead of the ElevatorStatus_t().

	return ElevatorStatus_t();
		
}