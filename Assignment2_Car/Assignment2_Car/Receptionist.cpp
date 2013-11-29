/*****************************************************
 * EECE 314 Assignment #2
 * Date: November 2013
 * Authors: 
 *         Kieren Wou		54724117
 *         Ryan Wong        47307103
 *****************************************************/
#include "Receptionist.h"

static int const k_numberOfCustomers = 2;

Receptionist::Receptionist(
	std::string firstName, 
	std::string lastName, 
	int age, 
	Gender_t gender, 
	TechnicianPtr_t const& pTechnician)
	: Person(firstName, lastName, age, gender)
	, m_pTechnician(pTechnician)
{
	m_pCarSemaphore = std::make_shared<CSemaphore>("TechRecepSemaphore",1);
}

Receptionist::~Receptionist()
{
	//delete m_pCarSemaphore;
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
	std::cout << "\rReceptionist: Finished making coffee\n";
}

void Receptionist::ReturnCar(CarPtr_t car, JobSheet jobSheet)
{
	std::cout << "Technician is handing over car keys to Receptionist";
	for(int i = 0; i < 4; ++i)
	{
		std::cout << ".";
	}
	std::cout << "\rCar has been returned to the Receptionist                    \n";
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
	std::cout << "Receptionist: Service Record has been stamped\n";
	svcRecord.UpdateServiceRecord(11212013, m_currentCarJobSheet);
	return svcRecord;
}

bool Receptionist::PayBill()
{
	std::cout << "Customer is paying bill";
	for(int i = 0; i < 4; ++i)
	{
		std::cout << ".";
	}
	std::cout << "\rBill has been paid            \n";

	return true;
}

int Receptionist::main()
{
	//CPipe TRPipe("TRPipe",1024);

	//CarPtr_t pFastCar = std::make_shared<Car>("Honda Civic", true); // hard coded

	//Car* pTempCarHolder;
	//JobSheet tempJobSheet;

	CPipe queuePipe("queue",1024);
	int count = 0;
	while(1)
	{
		Customer tempCustomer;
		if(queuePipe.TestForData() >= sizeof(Customer))
			queuePipe.Read(&tempCustomer, sizeof(Customer));

		m_pCustomer = &tempCustomer;
		if(!m_pCustomer)
			continue;

		m_pCustomer->ComeBackLater();
		
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
		//TRPipe.Read(pTempCarHolder, sizeof(Car));
		//TRPipe.Read(&tempJobSheet, sizeof(JobSheet));
	
		Invoice receipt = this->GenerateInvoice(11202013, m_currentCarJobSheet.TotalCost());
		ServiceRecord svcRec = this->StampSvcRecord(m_pCustomer->GetServiceRecord());

		m_pCustomer->ReturnServiceInfo(receipt, m_currentCarJobSheet, svcRec);

		this->PayBill();
		m_pCustomer->ReturnCar();

		CustomerPtr_t pTemp = m_pCustomer;
		m_pCustomer = NULL;

		if(++count == k_numberOfCustomers)
			break;
	}

	this->RequestTerminate();
	Sleep(2000); // delay to allow Technician::main() to exit loop

	return 0;
}

