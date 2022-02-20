#ifndef _MANAGER_H_
#define _MANAGER_H_

#include <string>
#include <vector>
#include <stdexcept>
#include <iostream>
#include "Person.h"

using namespace std;

class Manager: public Person
{   
	public:
	Manager(const string &name, int regNo); //constructor, sets the name and reg number
	int getRegNo() const; //gets a registration number for a manager obj
	void serialize(ostream &out); //writes a manager to the file

    private:
	int regNo; //registration number of manager
	string name; //name of manager
};

#endif
