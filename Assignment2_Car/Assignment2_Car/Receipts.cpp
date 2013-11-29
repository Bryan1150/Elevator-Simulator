/*****************************************************
 * EECE 314 Assignment #2
 * Date: November 2013
 * Authors: 
 *         Kieren Wou		54724117
 *         Ryan Wong        47307103
 *****************************************************/
#include "Receipts.h"

ServiceRecord::ServiceRecord()
{}

ServiceRecord::~ServiceRecord()
{
	m_dateToJobSheetMap.clear();
}

void ServiceRecord::UpdateServiceRecord(unsigned int date, JobSheet jobSheet)
{
	auto it = m_dateToJobSheetMap.find(date);
	if(it == m_dateToJobSheetMap.end())
		m_dateToJobSheetMap.insert(std::make_pair(date, jobSheet));
	else
		m_dateToJobSheetMap[date] = jobSheet;
}

Invoice::Invoice(unsigned int date, float cost)
	: m_date(date)
	, m_cost(cost)
{}

void Invoice::SetCost(float cost)
{
	m_cost = cost;
}

std::string Invoice::GetCostStr()
{
	std::stringstream ss;
	ss << m_cost;
	return ss.str();
}