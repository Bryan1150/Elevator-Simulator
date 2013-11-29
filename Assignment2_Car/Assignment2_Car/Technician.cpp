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
{
	m_pCurrentCar = NULL;

//	m_pReceptionist = std::make_shared<Receptionist>("Katherine", "Appleseed", 29, gender_female, (TechnicianPtr_t)this);
	m_pReceptionist = new Receptionist("Katherine", "Appleseed", 29, gender_female, (TechnicianPtr_t)this);
	m_pReceptionist->Resume();

	m_pCarSemaphore = std::make_shared<CSemaphore>("TechRecepSemaphore",1);
}

Technician::~Technician()
{
	//delete m_pCarSemaphore;
}

ReceptionistPtr_t Technician::GetReceptionist()
{
	return m_pReceptionist;
}

bool Technician::AvailableForNextCar() const
{
	return m_bAvailableForNextCar;
}

void Technician::Service(CarPtr_t pNextCar)
{
	std::cout << "Receptionist is passing car keys over to Technician\n";
	m_pCarSemaphore->Wait();
	m_pCurrentCar = pNextCar;
	m_pCarSemaphore->Signal();
}

int Technician::main()
{
//	Car fastCar("NFS", true);
	CPipe TRPipe("TRPipe",1024);

	while(1)
	{
		if(m_pCurrentCar)
		{
			m_bAvailableForNextCar = false;

			JobSheet jobSheet;
			
			Sleep(100);
			AirFilter oldAirFilter = m_pCurrentCar->SwapAirFilter(Store::GetAirFilter());
			Garbage::Dispose(oldAirFilter);

			Item item1(itemType_airFilter);
			jobSheet.AddItem(item1);
			Item item2(itemType_airFilterChange);
			jobSheet.AddItem(item2);

			Oil oldOil = m_pCurrentCar->SwapOil(Store::GetOil());
			Recycling::Recycle(oldOil);
	
			Item item5(itemType_oil);
			jobSheet.AddItem(item5);
			Item item6(itemType_oilChange);
			jobSheet.AddItem(item6);

			OilFilter oldOilFilter = m_pCurrentCar->SwapOilFilter(Store::GetOilFilter());
			Garbage::Dispose(oldOilFilter);

			Item item3(itemType_oilFilter);
			jobSheet.AddItem(item3);
			Item item4(itemType_oilFilterChange);
			jobSheet.AddItem(item4);

			if(m_pCurrentCar->CheckTiresForWear())
			{
				TireVect_t oldTires = m_pCurrentCar->SwapTires(Store::GetTire());
				Recycling::Recycle(*oldTires.begin());
		
				Item item8(itemType_tires);
				jobSheet.AddItem(item8);
				Item item7(itemType_swap);
				jobSheet.AddItem(item7);
			}
			else
			{
				m_pCurrentCar->RotateTires();
				Item item3(itemType_rotation);
				jobSheet.AddItem(item3);
			}

			//jobSheet.PrintJobSheet();

			m_bAvailableForNextCar = true;
			m_pReceptionist->ReturnCar(m_pCurrentCar, jobSheet);
			m_pCurrentCar = NULL;
		}
		if(m_pReceptionist->TerminateStatus())
			break;

	}

	m_pReceptionist->WaitForThread();
	delete m_pReceptionist;

	return 0;
}

