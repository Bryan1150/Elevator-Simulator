/*****************************************************
 * EECE 314 Assignment #2
 * Date: November 2013
 * Authors: 
 *         Kieren Wou		54724117
 *         Ryan Wong        47307103
 *****************************************************/
#include <iostream>

#include "rt.h"
#include "Person.h"
#include "AirFilter.h"
#include "OilFilter.h"
#include "Filter.h"
#include "Car.h"
#include "Tire.h"
#include "Store.h"
#include "Recycling.h"
#include "Garbage.h"
#include "JobSheet.h"

#pragma once

class Technician 
	: public Person
	, public ActiveClass 
{
public:
	Technician(std::string firstName, std::string lastName, int age, Gender_t gender);

	bool AvailableForNextCar() const;

private:
	int main();

	bool m_bAvailableForNextCar;
};