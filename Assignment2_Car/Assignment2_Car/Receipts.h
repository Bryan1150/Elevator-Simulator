/*****************************************************
 * EECE 314 Assignment #2
 * Date: November 2013
 * Authors: 
 *         Kieren Wou		54724117
 *         Ryan Wong        47307103
 *****************************************************/
#include <iostream>
#include <map>
#include <sstream>

#pragma once

#include "JobSheet.h"

typedef std::map<unsigned int/*date*/, JobSheet> DateToJobSheetMap_t;

class ServiceRecord {
public:
	ServiceRecord();
	~ServiceRecord();
	
	void UpdateServiceRecord(unsigned int date, JobSheet jobSheet);
	
	DateToJobSheetMap_t GetSRMap() const;

private:
	DateToJobSheetMap_t m_dateToJobSheetMap;
	
};

class Invoice {
public:
	Invoice(unsigned int date, float cost);

	void SetCost(float cost);
	std::string GetCostStr();

private:
	unsigned int m_date;
	float m_cost;
};