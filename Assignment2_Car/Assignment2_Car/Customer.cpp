/*****************************************************
 * EECE 314 Assignment #2
 * Date: November 2013
 * Authors: 
 *         Kieren Wou		54724117
 *         Ryan Wong        47307103
 *****************************************************/
#include "Customer.h"

static float const k_defaultBalance_dollars = 35293.29f;

Customer::Customer()
	: Person()
{}
Customer::Customer(std::string firstName, std::string lastName, int age, Gender_t gender, CarPtr_t car)
	: Person(firstName, lastName, age, gender)
	, m_cardBalance(k_defaultBalance_dollars)
	, m_serviceRecord(ServiceRecord())
	, m_pCar(car)
{}

Customer::~Customer()
{}

void Customer::ComeBackLater()
{
	std::cout << "Customer: Dropped off car+keys. Will come back later.\n";
}

void Customer::ReturnServiceInfo(Invoice receipt, JobSheet jobSheet, ServiceRecord svcRecord)
{
	std::cout << "Customer: Receiving invoice, job sheet and service record.\n";
	jobSheet.PrintJobSheet();
}

void Customer::ReturnCar()
{
	std::cout << "Customer: Receiving car+keys back from receptionist.\n";
}

CarPtr_t Customer::GetCar() const
{
	return m_pCar;
}

float Customer::GetCardBalance() const
{
	return m_cardBalance;
}

ServiceRecord Customer::GetServiceRecord() const
{
	return m_serviceRecord;
}
Customer Customer::operator=(Customer const& other)
{
	m_cardBalance = other.GetCardBalance();
	m_pCar = other.GetCar();
	m_serviceRecord = other.GetServiceRecord();

	return *this;
}
