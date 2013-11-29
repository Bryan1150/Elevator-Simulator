/*****************************************************
 * EECE 314 Assignment #2
 * Date: November 2013
 * Authors: 
 *         Kieren Wou		54724117
 *         Ryan Wong        47307103
 *****************************************************/
#include <iostream>

#pragma once

#include "Filter.h"

static std::string const k_genericAirFilter("\"NoNameAirFilter\"");


class AirFilter : public Filter {
public:
	AirFilter();

	std::string ToString() const;

private:
	std::string m_brand;

};