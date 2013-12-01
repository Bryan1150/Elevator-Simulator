/*****************************************************
 * EECE 314 Assignment #2
 * Date: November 2013
 * Authors: 
 *         Kieren Wou		54724117
 *         Ryan Wong        47307103
 *****************************************************/
#include <iostream>
#include <list>
#include <memory>

#include "Technician.h"
#include "Receptionist.h"
#include "Customer.h"
#include "rt.h"

int main(void)
{
	CSemaphore customerSemaphore("CustomerSemaphore",1);

	TechnicianPtr_t pTechnician = std::make_shared<Technician>("Bob", "Johnson", 35, gender_male);
	ReceptionistPtr_t pReceptionist= std::make_shared<Receptionist>("Katherine", "Appleseed", 29, gender_female);
	
	pTechnician->SetReceptionist(pReceptionist);
	pReceptionist->SetTechnician(pTechnician);
	
	pReceptionist->Resume();
	pTechnician->Resume();

	CarPtr_t pFastCar = std::make_shared<Car>("Honda Civic", true);
	CarPtr_t pWeirdCar = std::make_shared<Car>("BMW Z9", true);
	
	CustomerPtr_t pCustomer0 = std::make_shared<Customer>("Jimmy", "Tatro", 22, gender_male, pFastCar);
	CustomerPtr_t pCustomer1 = std::make_shared<Customer>("Christian", "Pierce", 23, gender_male, pWeirdCar);

	std::vector<CustomerPtr_t> customerVector;
	customerVector.push_back(pCustomer0);
	pCustomer0->ComeBackLater();
	customerVector.push_back(pCustomer1);
	pCustomer1->ComeBackLater();
	customerVector.push_back(pCustomer0);

	for(auto it = customerVector.begin(); it != customerVector.end(); ++it)
	{
		customerSemaphore.Wait();
		pReceptionist->SetCurrentCustomer(*it);
		customerSemaphore.Signal();
		Sleep(1000);
	} 

	pTechnician->WaitForThread();
	pReceptionist->WaitForThread();
	system("PAUSE");
	return 0;
}