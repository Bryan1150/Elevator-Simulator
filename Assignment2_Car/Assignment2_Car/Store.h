/*****************************************************
 * EECE 314 Assignment #2
 * Date: November 2013
 * Authors: 
 *         Kieren Wou		54724117
 *         Ryan Wong        47307103
 *****************************************************/
#include <iostream>

#include "AirFilter.h"
#include "OilFilter.h"
#include "Oil.h"
#include "Tire.h"

#pragma once

class Store {

public:
	static AirFilter GetAirFilter();
	static OilFilter GetOilFilter();
	static Oil GetOil();
	static Tire GetTire();

};