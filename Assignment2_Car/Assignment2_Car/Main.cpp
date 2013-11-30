/*****************************************************
 * EECE 314 Assignment #2
 * Date: November 2013
 * Authors: 
 *         Kieren Wou		54724117
 *         Ryan Wong        47307103
 *****************************************************/
#include <iostream>
#include <list>

#include "Technician.h"
#include "Customer.h"
#include "rt.h"

int main(void)
{
	CPipe queuePipe("queue",1024);

	Technician technician("Bob", "Johnson", 35, gender_male);
	technician.Resume();

	CarPtr_t pFastCar = std::make_shared<Car>("Honda Civic", true);
	CarPtr_t pWeirdCar = std::make_shared<Car>("BMW Z9", false);
	Customer customer0("Jimmy", "Tatro", 22, gender_male, pFastCar);
	Customer customer1("Christian", "Pierce", 23, gender_male, pWeirdCar);
	queuePipe.Write(&customer0, sizeof(Customer));
	queuePipe.Write(&customer1, sizeof(Customer));

	technician.WaitForThread();
	system("PAUSE");
	return 0;
}