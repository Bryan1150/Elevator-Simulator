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
	FigureOfSuitability_t fs = k_maximumElevators;

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
		{	
			if(floorReq.bInsideRequest) // next entry is an INSIDE request
			{
				if(((floorReq.direction == it->second.direction) && (abs(lift.floorNumber - it->second.floorNumber) > abs(lift.floorNumber - floorReq.floorNumber))) ||
					((floorReq.direction != it->second.direction) && (abs(lift.floorNumber - it->second.floorNumber) < abs(lift.floorNumber - floorReq.floorNumber))))
				{
					// only remove the previous outside FR if it is further away than the next inside FR
					lift.fsToFloorRequestMap.erase(it);
					lift.fsToFloorRequestMap.insert(std::make_pair(fs, floorReq));
				}
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
				if(lift.direction == k_directionUp)
				{
					if(it->second.floorNumber > lift.floorNumber && floorReq.floorNumber > lift.floorNumber)
					{	
						if(abs(lift.floorNumber - it->second.floorNumber) > abs(lift.floorNumber - floorReq.floorNumber))
						{
							// take the closer inside FR if both are in the same direction a the elevator
							lift.fsToFloorRequestMap.erase(it);
							lift.fsToFloorRequestMap.insert(std::make_pair(fs, floorReq));
						}
					}
					else if(it->second.floorNumber < lift.floorNumber)
					{
						// one of the two are in the opposite direction, so if the one in the map is below the elevator, then replace it
						lift.fsToFloorRequestMap.erase(it);
						lift.fsToFloorRequestMap.insert(std::make_pair(fs, floorReq));
					}
				}
				else if(lift.direction == k_directionDown)
				{
					if(it->second.floorNumber < lift.floorNumber && floorReq.floorNumber < lift.floorNumber)
					{
						if(abs(lift.floorNumber - it->second.floorNumber) > abs(lift.floorNumber - floorReq.floorNumber))
						{
							// take the closer inside FR if both are in the same direction a the elevator
							lift.fsToFloorRequestMap.erase(it);
							lift.fsToFloorRequestMap.insert(std::make_pair(fs, floorReq));
						}
					}
					else if(it->second.floorNumber > lift.floorNumber)
					{
						// one of the two are in the opposite direction, so if the one in the map is above the elevator, then replace it
						lift.fsToFloorRequestMap.erase(it);
						lift.fsToFloorRequestMap.insert(std::make_pair(fs, floorReq));
					}
				}
			}
			else // next entry is an OUTSIDE request
			{
				if(((floorReq.direction == it->second.direction) &&
					(abs(lift.floorNumber - it->second.floorNumber) > abs(lift.floorNumber - floorReq.floorNumber))) || 
					((floorReq.direction != it->second.direction) &&
					(abs(lift.floorNumber - it->second.floorNumber) < abs(lift.floorNumber - floorReq.floorNumber))))
				{
					// erase the inside FR that is already in map since the next FR is in the same direction
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
		bool bIsStartUp)
{
	auto itCheckForFault = floorRequestVect.begin();
	for(auto itCheckForFault = floorRequestVect.begin(); 
		itCheckForFault != floorRequestVect.end(); )
	{
		int elevatorIndex = itCheckForFault->elevatorId - 1;
		if(itCheckForFault->fReqId == k_faultFReqIdStr)
		{
			elevatorStatusVect[elevatorIndex].bFault = true;
		}
		else if(itCheckForFault->fReqId == k_clearFaultStr)
		{
			elevatorStatusVect[elevatorIndex].bFault = false;
			itCheckForFault = floorRequestVect.erase(itCheckForFault);
			continue;
		}
		++itCheckForFault;
	}

	if(!bIsStartUp)
	{
		// remove a floor request from the queue if it has already been serviced
		for(auto itElevatorStatus = elevatorStatusVect.begin(); 
			itElevatorStatus != elevatorStatusVect.end(); 
			++itElevatorStatus)
		{
			if(itElevatorStatus->bFault)
				continue; // we do nothing for an elevator if it has been signalled with a FAULT

			FloorRequest_t tempHolder;
			std::map<int/*floor*/,FloorRequest_t> duplicateFloorsList;
			for(auto itDeleteRequest = floorRequestVect.begin();
				itDeleteRequest != floorRequestVect.end();)
			{
				// remove from queue if FR is on same floor, door is open
				if((itDeleteRequest->floorNumber == itElevatorStatus->floorNumber) && 
					(itElevatorStatus->doorStatus == k_doorOpen))
				{	
					if(itDeleteRequest->bInsideRequest)
					{
						int elevIndex = std::distance(elevatorStatusVect.begin(), itElevatorStatus);
						
						if((itDeleteRequest->elevatorId - 1) == elevIndex)
							itDeleteRequest = floorRequestVect.erase(itDeleteRequest);
						else
							++itDeleteRequest;
						
						continue;
					}

					// elevator door would be open if an inside request was made on the same floor (ie. D3, 13) but
					// D3 should not be cleared if there are more floor requests in the elevator's current UP direction
					if(!itDeleteRequest->bInsideRequest && 
						(itDeleteRequest->direction != itElevatorStatus->direction) &&
						(itDeleteRequest->fReqId != k_idleFReqIdStr))
					{
						++itDeleteRequest;
						continue;
					}

					// if outside request, replace with pseudoFR to keep Elevator at that floor
					// so that the passenger can send an inside request to continue going in that direction
					auto it = duplicateFloorsList.find(itDeleteRequest->floorNumber);
					if(it == duplicateFloorsList.end())
					{
						duplicateFloorsList.insert(std::make_pair(itDeleteRequest->floorNumber,*itDeleteRequest));
						itDeleteRequest = floorRequestVect.erase(itDeleteRequest);
						continue;
					}
					else if(it->second.direction != itElevatorStatus->direction)
					{
						std::replace(floorRequestVect.begin(), floorRequestVect.end(), *itDeleteRequest, it->second);
					}
						
					++itDeleteRequest;					
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
			if(itElevatorStatus->bFault)
				continue;

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
				if(itFloorRequest->fReqId == k_faultFReqIdStr)
				{
					int elevatorIndex = std::distance(elevatorStatusVect.begin(), itElevatorStatus);
					if((itFloorRequest->elevatorId - 1) != elevatorIndex)
						continue;
				}
				
				if(itFloorRequest->bInsideRequest)
				{
					int elevatorIndex = std::distance(elevatorStatusVect.begin(), itElevatorStatus);
					assert(elevatorIndex >= 0 && elevatorIndex < 10);
					
					if((itFloorRequest->elevatorId - 1) == elevatorIndex) // only add for the proper elevator
					{
						itFloorRequest->direction = elevatorStatusVect[elevatorIndex].direction;
						FSAlgorithm::InsideRequest(*itFloorRequest, *itElevatorStatus);
					}
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
		for(auto itLift = elevatorStatusVect.begin(); itLift != elevatorStatusVect.end(); ++itLift)
		{	
			if(itLift->bFault)
			{
				int elevId = std::distance(elevatorStatusVect.begin(), itLift) + 1; // elevId is equal to elevIndex+1
				std::stringstream ss;
				ss << elevId;
				std::string elevIdStr = ss.str();
				FloorRequest_t faultFR(k_faultFReqIdStr+elevIdStr, elevId);
				floorReqToFsMap.insert(
					std::make_pair(faultFR,	std::make_pair(11/*fs*/, faultFR.elevatorId)));
				continue;
			}
			
			for(FsToFloorRequestMap_t::reverse_iterator itFs = itLift->fsToFloorRequestMap.rbegin();
				itFs != itLift->fsToFloorRequestMap.rend();
				++itFs)
			{
				if(((itLift->direction == k_directionUp) && (itFs->second.floorNumber < itLift->floorNumber)) || 
					((itLift->direction == k_directionDown) && (itFs->second.floorNumber > itLift->floorNumber)))
				{
					continue;
				}

				auto itMap = floorReqToFsMap.find(itFs->second/*floorRequest*/);
				if(itMap == floorReqToFsMap.end())
				{
					int elevatorId = std::distance(elevatorStatusVect.begin(), itLift) + 1; // NOTE: elevatorId equal to elevIndex+1
					floorReqToFsMap.insert(std::make_pair(itFs->second/*floorRequest*/, std::make_pair(itFs->first/*fs*/, elevatorId)));
					break;
				}
				else if(itFs->first > itMap->second.first/*fs*/) // floorRequest already in map, so if this next one has a larger FS, then replace the old one
				{	
					int elevRemovalIndex = itMap->second.second - 1;
				
					int elevatorId = std::distance(elevatorStatusVect.begin(), itLift) + 1;

					// erase then add new FR with the higher FS value
					floorReqToFsMap.erase(itMap);
					floorReqToFsMap.insert(
						std::make_pair(itLift->fsToFloorRequestMap.rbegin()->second/*floorRequest*/,
						std::make_pair(itLift->fsToFloorRequestMap.rbegin()->first/*fs*/, elevatorId)));
				
					// go through the remaining FR/FS for this elevator to find the next highest that is not in floorReqToFsMap
					for(auto itRemoval = elevatorStatusVect[elevRemovalIndex].fsToFloorRequestMap.rbegin();
						itRemoval != elevatorStatusVect[elevRemovalIndex].fsToFloorRequestMap.rend();
						++itRemoval)
					{
						if(floorReqToFsMap.find(itRemoval->second/*floorRequest*/) == floorReqToFsMap.end()) // if FR not yet in map, then insert
						{
							floorReqToFsMap.insert(
								std::make_pair(itRemoval->second/*floorRequest*/,
								std::make_pair(itRemoval->first/*fs*/, elevRemovalIndex+1)));
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
		// there might not be a FR for a certain elevator, so we send it an idle FR, telling it to remain in the same spot.
		// Elevator statuses from the DPs to the dispatcher are always stored in order of "elevatorId"

		FloorRequestVect_t outputDispatcher;
		outputDispatcher.reserve(k_maximumElevators);

		// initialize the outputDispatcher vector to have 10 default ElevatorStatus_t()
		for(int i = 0; i < elevatorStatusVect.size(); ++i)
		{
			outputDispatcher.push_back(FloorRequest_t());
		}

		for(auto iter = floorReqToFsMap.begin(); iter != floorReqToFsMap.end(); ++iter)
		{
			int elevIndex = iter->second.second - 1;
			if(elevIndex < outputDispatcher.size() && elevIndex >= 0) // safety check for index range
				outputDispatcher[elevIndex] = iter->first;
			else
				assert(false);
		}

		return outputDispatcher;
	}
	else
	{
		FloorRequestVect_t outputDispatcher;
		outputDispatcher.reserve(k_maximumElevators);

		FloorRequest_t startOnGroundLvl(0, k_directionUp);
		for(int i = 0; i < elevatorStatusVect.size(); ++i)
		{
			outputDispatcher.push_back(startOnGroundLvl);
		}

		return outputDispatcher;
	}
		
}