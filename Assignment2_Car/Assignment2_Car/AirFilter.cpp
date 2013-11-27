/*****************************************************
 * EECE 314 Assignment #2
 * Date: November 2013
 * Authors: 
 *         Kieren Wou		54724117
 *         Ryan Wong        47307103
 *****************************************************/
#include "AirFilter.h"

AirFilter::AirFilter()
	: Filter(0/*time*/, false/*bIsDisposed*/)
	, m_brand(k_genericAirFilter)
{}