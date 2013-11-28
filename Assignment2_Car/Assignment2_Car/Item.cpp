/*****************************************************
 * EECE 314 Assignment #2
 * Date: November 2013
 * Authors: 
 *         Kieren Wou		54724117
 *         Ryan Wong        47307103
 *****************************************************/
#include <sstream>
#include <iomanip>

#include "Item.h"

Item::Item(ItemTypes_t type)
	: m_type(type)
{
	switch(m_type)
	{
	case itemType_oil:
		m_cost = 5.0f;
		m_quantity = 1;
		break;
	case itemType_oilFilter:
		m_cost = 10.0f;
		m_quantity = 1;
		break;
	case itemType_airFilter:
		m_cost = 12.0f;
		m_quantity = 1;
		break;
	case itemType_oilChange:
		m_cost = 2.99f;
		break;
	case itemType_oilFilterChange:
		m_cost = 4.49f;
		break;
	case itemType_airFilterChange:
		m_cost = 4.49f;
		break;
	case itemType_tires:
		m_cost = 139.49f;
		m_quantity = 4;
		break;
	case itemType_rotation:
		m_cost = 14.49f;
		break;
	case itemType_swap:
		m_cost = 14.49f;
		break;
	default:
		break;
	}
}

float Item::GetCost()
{
	return m_cost;
}
std::string Item::ToString()
{
	std::stringstream cost;
	cost << std::setprecision(5) << m_cost;

	std::string itemStr = "Item: ";
	
	switch(m_type)
	{
	case itemType_oil:
		itemStr += "Oil Replacement\t\tQuantity: 1\tCost: $";
		break;
	case itemType_oilFilter:
		itemStr += "Oil Filter Replacement\tQuantity: 1\tCost: $";
		break;
	case itemType_airFilter:
		itemStr += "Air Filter Replacement\tQuantity: 1\tCost: $";
		break;
	case itemType_oilChange:
		itemStr += "Oil Change\t\tQuantity: n/a\tCost: $";
		break;
	case itemType_oilFilterChange:
		itemStr += "Oil Filter Change\t\tQuantity: n/a\tCost: $";
		break;
	case itemType_airFilterChange:
		itemStr += "Air Filter Change\t\tQuantity: n/a\tCost: $";
		break;
	case itemType_tires:
		itemStr += "Tire Replacement\t\tQuantity: 4\tCost: $";
		break;
	case itemType_rotation:
		itemStr += "Tire Rotation\t\tQuantity: n/a\tCost: $";
		break;
	case itemType_swap:
		itemStr += "Tires Changed\t\tQuantity: n/a\tCost: $";
		break;
	default:
		itemStr += "error in item type";
		break;
	}

	itemStr += cost.str();

	return itemStr;
}