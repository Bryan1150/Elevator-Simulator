/*****************************************************
 * EECE 314 Assignment #2
 * Date: November 2013
 * Authors: 
 *         Kieren Wou		54724117
 *         Ryan Wong        47307103
 *****************************************************/
#include "OilFilter.h"

OilFilter::OilFilter()
	: Filter(0/*time*/, false/*bIsDisposed*/)
	, m_brand(k_genericOilFilter)
{}