/*****************************************************
 * EECE 314 Assignment #2
 * Date: November 2013
 * Authors: 
 *         Kieren Wou		54724117
 *         Ryan Wong        47307103
 *****************************************************/
#include <iostream>

#pragma once

#include "Tire.h"
#include "Oil.h"


class Recycling {
public:
	template <class T>
	static void Recycle(T item)
	{
		std::cout << "Technician: Recycling " << item.ToString();
		for(int i = 0; i < 4; ++i)
		{
			Sleep(k_delay);
			std::cout << ".";
		}

		std::cout << "\rTechnician: " << item.ToString() << " finished being recycled.          \n";
	}
};
