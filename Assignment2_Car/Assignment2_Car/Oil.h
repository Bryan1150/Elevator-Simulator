/*****************************************************
 * EECE 314 Assignment #2
 * Date: November 2013
 * Authors: 
 *         Kieren Wou		54724117
 *         Ryan Wong        47307103
 *****************************************************/
#include <iostream>

#pragma once

static std::string const k_genericOil("NoNameOil");

class Oil {
public:
	Oil();

private:
	std::string m_brand;
	int m_lastChanged; // FIXME change to time struct
	bool m_bIsDisposed;
};