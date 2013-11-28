/*****************************************************
 * EECE 314 Assignment #2
 * Date: November 2013
 * Authors: 
 *         Kieren Wou		54724117
 *         Ryan Wong        47307103
 *****************************************************/
#include <iostream>

#pragma once

static unsigned int const k_small = 16;
static unsigned int const k_medium = 20;
static unsigned int const k_large = 24;
static std::string const k_goodWrench("\"Goodwrench\"");
static std::string const k_bridgestone("\"Bridgestone\"");
static std::string const k_goodyear("\"Goodyear\"");

class Tire {
public:
	Tire(bool bIsWorn = false);
	Tire(std::string brand, unsigned int size, bool bIsWorn = true);

	bool IsTireWorn() const;
	
	std::string ToString() const;

private:
	std::string m_brand;
	unsigned int m_size_inches;
	bool m_bIsWorn;
};