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

void FSAlgorithm::InsideRequest(
	FloorRequest_t const& floorReq,
	ElevatorStatus_t& lift)
{
	FigureOfSuitability_t fs = 10;

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
		if(!it->second.bInsideRequest) // map has an OUTSIDE request
		{	// INT_MAX is the default
			if(floorReq.bInsideRequest) // next entry is an INSIDE request, so overwrite map entry
			{
				lift.fsToFloorRequestMap.erase(it);
				lift.fsToFloorRequestMap.insert(std::make_pair(fs, floorReq));
			}
			else // next entry is *also* an OUTSIDE request, so take the closest floor to the elevator
			{
				if(it->second.direction != lift.direction && floorReq.direction == lift.direction)
				/*if(abs(lift.floorNumber - it->second.floorNumber) > abs(lift.floorNumber - floorReq.floorNumber))*/
				{
					lift.fsToFloorRequestMap.erase(it);
					lift.fsToFloorRequestMap.insert(std::make_pair(fs, floorReq));
				}
				// else, do nothing if the elevator is closer to the request that is already in the map
			}
		}
		else // map has an INSIDE request
		{
			if(floorReq.bInsideRequest) // next entry is an INSIDE request, so take the FR closest to the elevator's current floor
			{
				if(abs(lift.floorNumber - it->second.floorNumber) > abs(lift.floorNumber - floorReq.floorNumber))
				{
					lift.fsToFloorRequestMap.erase(it);
					lift.fsToFloorRequestMap.insert(std::make_pair(fs, floorReq));
				}
				// else, do nothing if the elevator is closer to the request that is already in the map
			}
			else // next entry is an OUTSIDE request
			{
				if((floorReq.direction == it->second.direction) &&
					(abs(lift.floorNumber - it->second.floorNumber) > abs(floorReq.floorNumber - floorReq.floorNumber)))
				{
					// erase the inside FR that is already in map since the next the next FR is in the same direction
					// and is closer to the elevator, so the inside FR would eventually be reached later on anyways
					lift.fsToFloorRequestMap.erase(it);
					lift.fsToFloorRequestMap.insert(std::make_pair(fs, floorReq));
				}
			}
		}
	}
}

// elevatorStatusVect contains DataPool struct from each elevator; passed into each Child Dispatcher Thread
// floorRequestVect contains all requests from IOProgram; need to be deleted by dispatcher once elevator(s) service the requests
FloorRequestVect_t FSAlgorithm::DispatcherFsCalculator(
		ElevatorStatusVect_t& elevatorStatusVect,
		FloorRequestVect_t& floorRequestVect,
		bool& bIsStartUp)
{
	if(!bIsStartUp)
	{
		// remove a floor request from the queue if it has already been serviced
		for(auto itElevatorStatus = elevatorStatusVect.begin(); 
			itElevatorStatus != elevatorStatusVect.end(); 
			++itElevatorStatus)
		{
			for(auto itDeleteRequest = floorRequestVect.begin();
				itDeleteRequest != floorRequestVect.end();)
			{
				if((itDeleteRequest->floorNumber == itElevatorStatus->floorNumber) && 
					(itElevatorStatus->doorStatus == k_doorOpen) /*&&
					(itDeleteRequest->direction == itElevatorStatus->direction)*/)
				{	// only rm from queue if FR is on same floor, door is open
				
					// if outside request, replace with pseudoFR to keep Elevator at that floor
					// so that the passenger can send an inside request to continue going in that direction
// 					if(!itDeleteRequest->bInsideRequest) 
// 					{
// 						FloorRequest_t pseudoFR(itDeleteRequest->floorNumber, 
// 							itDeleteRequest->direction,
// 							std::distance(elevatorStatusVect.begin(), itElevatorStatus));
// 
// 						std::replace(floorRequestVect.begin(), floorRequestVect.end(), *itDeleteRequest, pseudoFR);
// 
// 						++itDeleteRequest;
// 					}
// 					else
// 					{
						itDeleteRequest = floorRequestVect.erase(itDeleteRequest);
//					}
				}
				else
				{
					++itDeleteRequest;
				}
			}
		}


		for(ElevatorStatusVect_t::iterator itElevatorStatus = elevatorStatusVect.begin();
			itElevatorStatus != elevatorStatusVect.end();
			++itElevatorStatus)
		{
			if(floorRequestVect.empty())
			{
				if(itElevatorStatus->floorNumber == 0)
					itElevatorStatus->direction = k_directionUp;
				else if(itElevatorStatus->floorNumber == 9)
					itElevatorStatus->direction = k_directionDown;
				else
					itElevatorStatus->direction = k_directionIdle;
	
				FloorRequest_t idleFloorReq(
					itElevatorStatus->floorNumber,
					itElevatorStatus->direction,
					k_idleFReqIdStr);

				floorRequestVect.push_back(idleFloorReq);
			}
		
			for(auto itFloorRequest = floorRequestVect.begin();
				itFloorRequest != floorRequestVect.end();
				++itFloorRequest)
			{
				if(itFloorRequest->bInsideRequest)
				{
					int elevId = itFloorRequest->elevatorId;
					assert(elevId > 0 && elevId <= 10);
					itFloorRequest->direction = elevatorStatusVect[elevId-1].direction;

					FSAlgorithm::InsideRequest(*itFloorRequest, *itElevatorStatus);
				}

				else if(itElevatorStatus->direction == k_directionIdle)
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

		std::map<FloorRequest_t, std::pair<FigureOfSuitability_t,int/*elevatorId*/>> floorReqToFsMap;
		std::vector<int> reorderQueue;
		auto itLift = elevatorStatusVect.begin();
		for(itLift; itLift != elevatorStatusVect.end(); ++itLift)
		{	
			for(auto itFs = itLift->fsToFloorRequestMap.rbegin();
				itFs != itLift->fsToFloorRequestMap.rend();
				++itFs)
			{
				auto itMap = floorReqToFsMap.find(itFs->second/*floorRequest*/);
				if(itMap == floorReqToFsMap.end())
				{
					int elevatorId = std::distance(elevatorStatusVect.begin(), itLift); // NOTE: elevatorId is the index beginning from 0
					floorReqToFsMap.insert(std::make_pair(itFs->second/*floorRequest*/, std::make_pair(itFs->first/*fs*/, elevatorId)));
					break;
				}
				else if(itFs->first > itMap->second.first/*fs*/) // floorRequest already in map, so if this next one has a larger FS, then replace the old one
				{	
					int elevatorRemovalId = itMap->second.second;
				
					int elevatorId = std::distance(elevatorStatusVect.begin(), itLift);

					// erase then add new FR with the higher FS value
					floorReqToFsMap.erase(itMap);
					floorReqToFsMap.insert(
						std::make_pair(itLift->fsToFloorRequestMap.rbegin()->second/*floorRequest*/,
						std::make_pair(itLift->fsToFloorRequestMap.rbegin()->first/*fs*/, elevatorId)));
				
					// go through the remaining FR/FS for this elevator to find the next highest that is not in floorReqToFsMap
					for(auto itRemoval = elevatorStatusVect[elevatorRemovalId].fsToFloorRequestMap.rbegin();
						itRemoval != elevatorStatusVect[elevatorRemovalId].fsToFloorRequestMap.rend();
						++itRemoval)
					{
						if(floorReqToFsMap.find(itRemoval->second/*floorRequest*/) == floorReqToFsMap.end()) // if FR not yet in map, then insert
						{
							floorReqToFsMap.insert(
								std::make_pair(itRemoval->second/*floorRequest*/,
								std::make_pair(itRemoval->first/*fs*/, elevatorRemovalId)));
						
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
		// Create a final vector containing the FR for each elevator in order; return this vector instead of the ElevatorStatus_t().

		FloorRequestVect_t outputDispatcher;
		outputDispatcher.reserve(10);

		// initialize the outputDispatcher vector to have 10 default ElevatorStatus_t()
		for(int i = 0; i < elevatorStatusVect.size(); ++i)
		{
			outputDispatcher.push_back(FloorRequest_t());
		}

		for(auto iter = floorReqToFsMap.begin(); iter != floorReqToFsMap.end(); ++iter)
		{
			int elevatorId = iter->second.second;
			if(elevatorId < outputDispatcher.size() && elevatorId >= 0) // safety check for index range
				outputDispatcher[elevatorId] = iter->first;
			else
				assert(false);
		}

		return outputDispatcher;
	}
	else
	{
		FloorRequestVect_t outputDispatcher;
		outputDispatcher.reserve(10);

		FloorRequest_t startOnGroundLvl(0, k_directionUp);
		for(int i = 0; i < elevatorStatusVect.size(); ++i)
		{
			outputDispatcher.push_back(startOnGroundLvl);
		}

		return outputDispatcher;
	}
		
}