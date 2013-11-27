/*****************************************************
 * EECE 314 Assignment #2
 * Date: November 2013
 * Authors: 
 *         Kieren Wou		54724117
 *         Ryan Wong        47307103
 *****************************************************/
#include "Filter.h"

Filter::Filter(int lastChanged, bool bIsDisposed)
	: m_lastChanged(lastChanged)
	, m_bIsDisposed(bIsDisposed)
{}

void Filter::SetBoolIsDisposed()
{
	m_bIsDisposed = true;
}

void Filter::SetTimeOfLastChanged()
{
	m_lastChanged = -1; // FIXME use proper time struct
}