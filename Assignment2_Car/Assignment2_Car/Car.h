/*****************************************************
 * EECE 314 Assignment #2
 * Date: November 2013
 * Authors: 
 *         Kieren Wou		54724117
 *         Ryan Wong        47307103
 *****************************************************/
#include <iostream>
#include <vector>

#pragma once

#include "rt.h"
#include "Tire.h"
#include "AirFilter.h"
#include "OilFilter.h"
#include "Oil.h"

typedef std::vector<Tire> TireVect_t;
typedef std::shared_ptr<AirFilter> AirFilterPtr_t;
typedef std::shared_ptr<OilFilter> OilFilterPtr_t;
typedef std::shared_ptr<Oil> OilPtr_t;

class Car {

public:
	Car(std::string model, bool bWornTires);
	
	Oil SwapOil(Oil newOil);
	AirFilter SwapAirFilter(AirFilter newAirFilter);
	OilFilter SwapOilFilter(OilFilter newOilFilter);

	Car operator=(Car const& other);

	// accessors
	TireVect_t GetTireVect() const;
	std::string GetModelName() const;

	bool CheckTiresForWear() const;
	TireVect_t SwapTires(Tire newTire);
	void RotateTires();

private:
	std::string m_model;
	AirFilter m_airFilter;
	OilFilter m_oilFilter;
	Oil m_oil;
	TireVect_t m_tireVect;
	
};