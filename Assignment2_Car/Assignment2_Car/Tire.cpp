/*****************************************************
 * EECE 314 Assignment #2
 * Date: November 2013
 * Authors: 
 *         Kieren Wou		54724117
 *         Ryan Wong        47307103
 *****************************************************/
#include "Tire.h"

Tire::Tire(bool bIsWorn)
	: m_brand(k_bridgestone)
	, m_size_inches(k_medium)
	, m_bIsWorn(bIsWorn)
{}

Tire::Tire(std::string brand, unsigned int size, bool bIsWorn)
	: m_brand(brand)
	, m_size_inches(size)
	, m_bIsWorn(bIsWorn)
{}

bool Tire::IsTireWorn() const
{
	return m_bIsWorn;
}