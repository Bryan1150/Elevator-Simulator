/*****************************************************
 * EECE 314 Assignment #2
 * Date: November 2013
 * Authors: 
 *         Kieren Wou		54724117
 *         Ryan Wong        47307103
 *****************************************************/
#include "Technician.h"

Technician::Technician(std::string firstName, std::string lastName, int age, Gender_t gender)
	: Person(firstName, lastName, age, gender)
{}

bool Technician::AvailableForNextCar() const
{
	return m_bAvailableForNextCar;
}

int Technician::main()
{
	Car fastCar("NFS", true);
	JobSheet jobSheet;
	
	AirFilter oldAirFilter = fastCar.SwapAirFilter(Store::GetAirFilter());
	Garbage::Dispose(oldAirFilter);

	Item item1(itemType_airFilter);
	jobSheet.AddItem(item1);
	Item item2(itemType_airFilterChange);
	jobSheet.AddItem(item2);

	Oil oldOil = fastCar.SwapOil(Store::GetOil());
	Recycling::Recycle(oldOil);
	
	Item item5(itemType_oil);
	jobSheet.AddItem(item5);
	Item item6(itemType_oilChange);
	jobSheet.AddItem(item6);

	OilFilter oldOilFilter = fastCar.SwapOilFilter(Store::GetOilFilter());
	Garbage::Dispose(oldOilFilter);

	Item item3(itemType_oilFilter);
	jobSheet.AddItem(item3);
	Item item4(itemType_oilFilterChange);
	jobSheet.AddItem(item4);

	if(fastCar.CheckTiresForWear())
	{
		TireVect_t oldTires = fastCar.SwapTires(Store::GetTire());
		Recycling::Recycle(*oldTires.begin());
		
		Item item8(itemType_tires);
		jobSheet.AddItem(item8);
		Item item7(itemType_swap);
		jobSheet.AddItem(item7);
	}
	else
	{
		fastCar.RotateTires();
		Item item3(itemType_rotation);
		jobSheet.AddItem(item3);
	}

	jobSheet.PrintJobSheet();

	system("PAUSE");
	return 0;
}

