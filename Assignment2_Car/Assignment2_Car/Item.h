/*****************************************************
 * EECE 314 Assignment #2
 * Date: November 2013
 * Authors: 
 *         Kieren Wou		54724117
 *         Ryan Wong        47307103
 *****************************************************/
#include <iostream>

#pragma once

enum ItemTypes_t {
	itemType_oil,
	itemType_oilFilter,
	itemType_airFilter,
	itemType_oilChange,
	itemType_oilFilterChange,
	itemType_airFilterChange,
	itemType_tires,
	itemType_rotation,
	itemType_swap
};

class Item {
public:
	Item(ItemTypes_t type);

	float GetCost();
	std::string ToString();

private:
	ItemTypes_t m_type;
	float m_cost;
	int m_quantity;
};