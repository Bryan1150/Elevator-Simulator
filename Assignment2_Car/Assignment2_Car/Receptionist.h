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
#include "Receipts.h"
#include "Car.h"
#include "Customer.h"
#include "JobSheet.h"
#include "Technician.h"

class Technician;
typedef std::shared_ptr<Technician> TechnicianPtr_t;

class JobSheet;
typedef std::shared_ptr<JobSheet> JobSheetPtr_t;

class Customer;
typedef std::shared_ptr<Customer> CustomerPtr_t;
//typedef Customer* CustomerPtr_t;

class Car;
typedef std::shared_ptr<Car> CarPtr_t;

class Receptionist
	: public Person
	, public ActiveClass
{
public:
	Receptionist(
		std::string firstName, 
		std::string lastName, 
		int age, 
		Gender_t gender);

	~Receptionist();

	void SetTechnician(TechnicianPtr_t const& pTechnician);
	void SetCurrentCustomer(CustomerPtr_t const& pCustomer);
	void ReleaseCurrentCustomer();
	CustomerPtr_t GetCurrentCustomer() const;

	Car ServiceCar(Car& car);
	void MakeCoffee();
	void ReturnCar(CarPtr_t car, JobSheet jobSheet);
	Invoice GenerateInvoice(unsigned int date, float cost);
	ServiceRecord StampSvcRecord(ServiceRecord svcRecord);
	bool PayBill();

private:
	int main();

	TechnicianPtr_t m_pTechnician;
	CustomerPtr_t m_pCustomer;
	JobSheet m_currentCarJobSheet;
	std::shared_ptr<CSemaphore> m_pCarSemaphore;
	std::shared_ptr<CSemaphore> m_pCustomerSemaphore;
	bool m_bCarIsReturned;
};