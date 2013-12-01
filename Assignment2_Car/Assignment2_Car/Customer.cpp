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

Customer::Customer(std::string firstName, std::string lastName, int age, Gender_t gender, CarPtr_t const& pCar)
	: Person(firstName, lastName, age, gender)
	, m_cardBalance(k_defaultBalance_dollars)
	, m_serviceRecord(ServiceRecord())
	, m_pCar(pCar)
{}

Customer::~Customer()
{}

void Customer::ComeBackLater()
{
	std::cout << "Customer (" << this->PersonNameToString() << "): Dropped off car+keys. Will come back later.\n";
	Sleep(k_delay);
}

void Customer::ReturnServiceInfo(Invoice receipt, JobSheet jobSheet, ServiceRecord svcRecord)
{
	std::cout << "Customer(" << this->PersonNameToString() << "): Receiving invoice, job sheet and service record.\n";
	jobSheet.PrintJobSheet();
}

void Customer::ReturnCar()
{
	std::cout << "Customer(" << this->PersonNameToString() << "): Receiving car+keys back from receptionist.\n";
	Sleep(k_delay);
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
	if(!other.GetServiceRecord().GetSRMap().empty())
		m_serviceRecord = other.GetServiceRecord();
	m_cardBalance = other.GetCardBalance();
	m_pCar = other.GetCar();

	return *this;
}
