/*****************************************************
 * EECE 314 Assignment #2
 * Date: November 2013
 * Authors: 
 *         Kieren Wou		54724117
 *         Ryan Wong        47307103
 *****************************************************/
#include "Car.h"

Car::Car(std::string model, bool bWornTires)
	: m_model(model)
	, m_airFilter(AirFilter())
	, m_oilFilter(OilFilter())
	, m_oil(Oil())
{
	m_tireVect.reserve(4);
	for(int i = 0; i < 4; ++i)
	{
		m_tireVect.push_back(Tire(bWornTires));
	}
}

Car Car::operator=(Car const& other)
{
	m_tireVect = other.GetTireVect();
	m_model = other.GetModelName();
	
	return *this;
}

TireVect_t Car::GetTireVect() const
{
	return m_tireVect;
}

std::string Car::GetModelName() const
{
	return m_model;
}

Oil Car::SwapOil(Oil newOil)
{
	Oil oldOil = m_oil;
	m_oil = newOil;

	std::cout << "Technician: Swapping oil";
	for(int i = 0; i < 4; ++i)
	{
		Sleep(k_delay);
		std::cout << ".";
	}
	std::cout << "\rTechnician: Finished swapping oil.         \n";

	return oldOil;
}

AirFilter Car::SwapAirFilter(AirFilter newAirFilter)
{
	AirFilter oldAirFilter = m_airFilter;
	m_airFilter = newAirFilter;

	std::cout << "Technician: Swapping air filter";
	for(int i = 0; i < 4; ++i)
	{
		Sleep(k_delay);
		std::cout << ".";
	}
	std::cout << "\rTechnician: Finished swapping air filter.           \n";

	return oldAirFilter;
}

OilFilter Car::SwapOilFilter(OilFilter newOilFilter)
{
	OilFilter oldOilFilter = m_oilFilter;
	m_oilFilter = newOilFilter;

	std::cout << "Technician: Swapping oil filter";
	for(int i = 0; i < 4; ++i)
	{
		Sleep(k_delay);
		std::cout << ".";
	}
	std::cout << "\rTechnician: Finished swapping oil filter.               \n";

	return oldOilFilter;
}

bool Car::CheckTiresForWear() const
{
	std::cout << "Technician: Checking tires for wear";
	bool bTiresWorn = false;
	for(auto it = m_tireVect.begin(); it != m_tireVect.end(); ++it)
	{
		Sleep(k_delay);
		std::cout << ".";

		bTiresWorn |= it->IsTireWorn(); // if at least one tire is worn, return true
	}

	bTiresWorn 
		? std::cout << "\rTechnician: Tires are worn. Need replacing.           \n" 
		: std::cout << "\rTechnician: Tires are not worn. Need rotating.        \n";
	
	Sleep(1300);
	return bTiresWorn;
}

TireVect_t Car::SwapTires(Tire newTire)
{
	std::cout << "Technician: Swapping tires";
	TireVect_t oldTires = m_tireVect;
	
	m_tireVect.clear();
	
	for(int i = 0; i < 4; ++i)
	{
		m_tireVect.push_back(newTire);
		Sleep(k_delay);
		std::cout << ".";
	}
	std::cout << "\rTechnician: Finished swapping tires.          \n";
	return oldTires;
}

void Car::RotateTires()
{
	std::cout << "Technician: Rotating tires";
	for(int i = 0; i < 4; ++i)
	{
		Sleep(k_delay);
		std::cout << ".";
	}
	std::cout << "\rTechnician: Finished rotating tires.       \n";
}