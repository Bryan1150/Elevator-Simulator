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
#include "Store.h"
#include "Recycling.h"
#include "Garbage.h"

int main()
{
	Car fastCar("NFS", true);

	AirFilter oldAirFilter = fastCar.SwapAirFilter(Store::GetAirFilter());
	Garbage::Dispose(oldAirFilter);

	Oil oldOil = fastCar.SwapOil(Store::GetOil());
	Recycling::Recycle(oldOil);
	
	OilFilter oldOilFilter = fastCar.SwapOilFilter(Store::GetOilFilter());
	Garbage::Dispose(oldOilFilter);

	if(fastCar.CheckTiresForWear())
	{
		TireVect_t oldTires = fastCar.SwapTires(Store::GetTire());
		Recycling::Recycle(*oldTires.begin());
	}
	else
	{
		fastCar.RotateTires();
	}

	return 0;
}