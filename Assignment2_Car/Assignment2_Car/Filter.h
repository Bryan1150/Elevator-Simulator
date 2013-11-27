/*****************************************************
 * EECE 314 Assignment #2
 * Date: November 2013
 * Authors: 
 *         Kieren Wou		54724117
 *         Ryan Wong        47307103
 *****************************************************/
#include <iostream>

#pragma once

class Filter {
public:
	Filter(int lastChanged, bool bIsDisposed = false);
	
	void SetTimeOfLastChanged();
	void SetBoolIsDisposed();

private:
	int m_lastChanged; // FIXME use proper time struct
	bool m_bIsDisposed;
};