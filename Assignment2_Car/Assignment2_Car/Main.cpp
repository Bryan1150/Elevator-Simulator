/*****************************************************
 * EECE 314 Assignment #2
 * Date: November 2013
 * Authors: 
 *         Kieren Wou		54724117
 *         Ryan Wong        47307103
 *****************************************************/
#include <iostream>

#include "Technician.h"
#include "rt.h"

int main(void)
{
	Technician technician("Bob", "Johnson", 35, gender_male);
	technician.Resume();

	technician.WaitForThread();
	return 0;
}