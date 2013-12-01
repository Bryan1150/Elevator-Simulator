/*****************************************************
 * EECE 314 Assignment #2
 * Date: November 2013
 * Authors: 
 *         Kieren Wou		54724117
 *         Ryan Wong        47307103
 *****************************************************/
#include <iostream>

#pragma once

#include "rt.h"
#include "Person.h"
#include "JobSheet.h"
#include "Receipts.h"
#include "Car.h"

class Car;
typedef std::shared_ptr<Car> CarPtr_t;

class Customer
	: public Person
{
public:
	Customer();
	Customer(std::string firstName, std::string lastName, int age, Gender_t gender, CarPtr_t const& pCar);
	~Customer();

	void ComeBackLater();
	void ReturnServiceInfo(Invoice receipt, JobSheet jobsheet, ServiceRecord svcRecord);
	void ReturnCar();

	// accessors
	CarPtr_t GetCar() const;
	float GetCardBalance() const;
	ServiceRecord GetServiceRecord() const;

	Customer operator=(Customer const& other);
private:

	float m_cardBalance;
	ServiceRecord m_serviceRecord;
	CarPtr_t m_pCar;
};