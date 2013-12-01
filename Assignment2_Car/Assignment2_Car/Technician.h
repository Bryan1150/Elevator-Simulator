/*****************************************************
 * EECE 314 Assignment #2
 * Date: November 2013
 * Authors: 
 *         Kieren Wou		54724117
 *         Ryan Wong        47307103
 *****************************************************/
#include <iostream>
#include <memory>

#pragma once

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
#include "Receptionist.h"

class Receptionist;
typedef std::shared_ptr<Receptionist> ReceptionistPtr_t;

class Car;
typedef std::shared_ptr<Car> CarPtr_t;

class Technician 
	: public Person
	, public ActiveClass 
{
public:
	Technician(std::string firstName, std::string lastName, int age, Gender_t gender);
	~Technician();

	ReceptionistPtr_t GetReceptionist();
	void SetReceptionist(ReceptionistPtr_t const& pReceptionist);

	bool AvailableForNextCar() const;
	void Service(CarPtr_t const& pNextCar);

private:
	int main();

	bool m_bAvailableForNextCar;
	ReceptionistPtr_t m_pReceptionist;
	CarPtr_t m_pCurrentCar;
	std::shared_ptr<CSemaphore> m_pCarSemaphore;
};