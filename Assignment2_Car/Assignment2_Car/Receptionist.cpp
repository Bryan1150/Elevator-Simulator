/*****************************************************
 * EECE 314 Assignment #2
 * Date: November 2013
 * Authors: 
 *         Kieren Wou		54724117
 *         Ryan Wong        47307103
 *****************************************************/
#include "Receptionist.h"

static int const k_numberOfCustomers = 3;

Receptionist::Receptionist(
	std::string firstName, 
	std::string lastName, 
	int age, 
	Gender_t gender)
	: Person(firstName, lastName, age, gender)
{
	m_pTechnician = NULL;
	m_pCarSemaphore = std::make_shared<CSemaphore>("TechRecepSemaphore",1);
	m_pCustomerSemaphore = std::make_shared<CSemaphore>("CustomerSemaphore",1);
}

Receptionist::~Receptionist()
{}

void Receptionist::SetTechnician(TechnicianPtr_t const& pTechnician)
{
	m_pTechnician = pTechnician;
}

void Receptionist::SetCurrentCustomer(CustomerPtr_t const& pCustomer)
{
	m_pCustomer = pCustomer;
}

void Receptionist::ReleaseCurrentCustomer()
{
	m_pCustomer = NULL;
}

CustomerPtr_t Receptionist::GetCurrentCustomer() const
{
	return m_pCustomer;
}

Car Receptionist::ServiceCar(Car& car)
{
	return car;
}

void Receptionist::MakeCoffee()
{
	std::cout << "Receptionist: Making coffee";
	for(int i = 0; i < 4; ++i)
	{
		std::cout << ".";
	}
	std::cout << "\rReceptionist: Finished making coffee.\n";
}

void Receptionist::ReturnCar(CarPtr_t car, JobSheet jobSheet)
{
	std::cout << "Technician: handing over " << m_pCustomer->PersonNameToString() << "'s car keys to Receptionist";
	for(int i = 0; i < 4; ++i)
	{
		std::cout << ".";
	}
	std::cout << "\rTechnician: " << m_pCustomer->PersonNameToString() << "'s car keys have been returned to the Receptionist. \n";
	m_currentCarJobSheet = jobSheet;
	m_bCarIsReturned = true;
}

Invoice Receptionist::GenerateInvoice(unsigned int date, float cost)
{
	Invoice invoice(date, cost);
	std::cout << "Receptionist: Invoice printed. Total cost: $" << invoice.GetCostStr() << std::endl;

	return invoice;
}

ServiceRecord Receptionist::StampSvcRecord(ServiceRecord svcRecord)
{
	std::cout << "Receptionist: Service Record has been stamped.\n";
	svcRecord.UpdateServiceRecord(11212013, m_currentCarJobSheet);
	return svcRecord;
}

bool Receptionist::PayBill()
{
	std::cout << "Customer(" << this->PersonNameToString() << "): Paying bill";
	for(int i = 0; i < 4; ++i)
	{
		std::cout << ".";
	}
	std::cout << "\rCustomer(" << this->GetCurrentCustomer()->PersonNameToString() << "): Bill has been paid.            \n";

	return true;
}

int Receptionist::main()
{
	int count = 0;
	while(1)
	{
		if(!m_pCustomer)
			continue;

		m_pCustomerSemaphore->Wait();
		
		std::cout << std::endl;

		if(m_pTechnician->AvailableForNextCar())
		{
			m_pTechnician->Service(m_pCustomer->GetCar());
			m_bCarIsReturned = false;
		}

		this->MakeCoffee();

		bool bExitLoop = false;
		while(!bExitLoop)
		{
			if(m_bCarIsReturned)
				bExitLoop = true;
		}

		Invoice receipt = this->GenerateInvoice(11202013, m_currentCarJobSheet.TotalCost());
		ServiceRecord svcRec = this->StampSvcRecord(m_pCustomer->GetServiceRecord());

		m_pCustomer->ReturnServiceInfo(receipt, m_currentCarJobSheet, svcRec); // jobSheet is printed

		this->PayBill();
		m_pCustomer->ReturnCar();
		this->ReleaseCurrentCustomer();

		std::cout << std::endl;

		m_pCustomerSemaphore->Signal();

		if(++count == k_numberOfCustomers)
			break;
	}

	this->RequestTerminate();

	return 0;
}

