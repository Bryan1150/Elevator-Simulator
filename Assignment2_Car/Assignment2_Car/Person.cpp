/*****************************************************
 * EECE 314 Assignment #2
 * Date: November 2013
 * Authors: 
 *         Kieren Wou		54724117
 *         Ryan Wong        47307103
 *****************************************************/
#include "Person.h"

Person::Person()
{}

Person::Person(std::string firstName, std::string lastName, int age, Gender_t gender)
	: m_firstName(firstName)
	, m_lastName(lastName)
	, m_age(age)
	, m_gender(gender)
{}

Person::~Person()
{}