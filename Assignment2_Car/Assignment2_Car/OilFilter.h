/*****************************************************
 * EECE 314 Assignment #2
 * Date: November 2013
 * Authors: 
 *         Kieren Wou		54724117
 *         Ryan Wong        47307103
 *****************************************************/
#include <iostream>

#include "Filter.h"

#pragma once

static std::string const k_genericOilFilter("NoNameOilFilter");

class OilFilter : public Filter {
public:
	OilFilter();

private:
	std::string m_brand;

};