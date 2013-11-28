/*****************************************************
 * EECE 314 Assignment #2
 * Date: November 2013
 * Authors: 
 *         Kieren Wou		54724117
 *         Ryan Wong        47307103
 *****************************************************/
#include <iostream>

#pragma once

class Garbage {
public:
	template<class T>
	static void Dispose(T item)
	{
		std::cout << "Disposing of " << item.ToString();
		for(int i = 0; i < 4; ++i)
		{
			Sleep(k_delay);
			std::cout << ".";
		}

		std::cout << "\r" << item.ToString() << " has been disposed.\n";
	}
};