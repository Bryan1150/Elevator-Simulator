/*****************************************************
 * EECE 314 Assignment #2
 * Date: November 2013
 * Authors: 
 *         Kieren Wou		54724117
 *         Ryan Wong        47307103
 *****************************************************/
#include <iostream>
#include <vector>

#pragma once

#include "Item.h"

class JobSheet {
public:
	JobSheet();
	
	void AddItem(Item item);
	void PrintJobSheet();
	float TotalCost();

private:
	//std::list<Item> m_itemList;
	//std::list<Item>::iterator m_iterator;
	std::vector<Item> m_itemVect;
};