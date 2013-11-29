/*****************************************************
 * EECE 314 Assignment #2
 * Date: November 2013
 * Authors: 
 *         Kieren Wou		54724117
 *         Ryan Wong        47307103
 *****************************************************/
#include <iostream>

#pragma once

enum Gender_t {
	gender_male,
	gender_female
};

class Person {

public:
	Person(std::string firstName, std::string lastName, int age, Gender_t gender);	

private:
	std::string m_firstName;
	std::string m_lastName;
	int m_age;
	Gender_t m_gender;

};