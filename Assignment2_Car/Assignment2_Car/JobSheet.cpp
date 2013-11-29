/*****************************************************
 * EECE 314 Assignment #2
 * Date: November 2013
 * Authors: 
 *         Kieren Wou		54724117
 *         Ryan Wong        47307103
 *****************************************************/
#include <string>
#include "JobSheet.h"

JobSheet::JobSheet()
{}

void JobSheet::AddItem(Item item)
{
	m_itemVect.push_back(item);
}

void JobSheet::PrintJobSheet()
{
	std::cout << "Job Sheet:\n=============================================================" << std::endl;
	for(auto it = m_itemVect.begin();
		it != m_itemVect.end();
		++it)
	{
		std::cout << it->ToString() << std::endl;
	}
	std::cout << "=============================================================" << std::endl;
}

float JobSheet::TotalCost()
{
	float totalCost = 0.0f;
	for(auto it = m_itemVect.begin();
		it != m_itemVect.end();
		++it)
	{
		totalCost += it->GetCost();
	}
	
	return totalCost;
}