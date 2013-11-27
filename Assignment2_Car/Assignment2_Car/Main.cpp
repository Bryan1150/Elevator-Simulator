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
#include "Filter.h"
#include "Car.h"
#include "Tire.h"


int main()
{
	Car fastCar("NFS", false);
	fastCar.SwapAirFilter(AirFilter());
	fastCar.SwapOil(Oil());
	fastCar.SwapOilFilter(OilFilter());
	if(fastCar.CheckTiresForWear())
		fastCar.SwapTires(Tire());
	else
		fastCar.RotateTires();
	return 0;
}