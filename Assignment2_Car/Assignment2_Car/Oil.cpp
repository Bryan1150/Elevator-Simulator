/*****************************************************
 * EECE 314 Assignment #2
 * Date: November 2013
 * Authors: 
 *         Kieren Wou		54724117
 *         Ryan Wong        47307103
 *****************************************************/
#include "Oil.h"

Oil::Oil()
	: m_brand(k_genericOil)
	, m_lastChanged(0) // FIXME to time 
	, m_bIsDisposed(false)
{}