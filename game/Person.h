#ifndef _PERSON_H_
#define _PERSON_H_

#include <string>
#include <stdexcept>

using namespace std;
//base class used for managers and gamers

class Person
{   public:
		Person(const string &name);
	    string getName() const;
	    void changeName(const string &newName);
		virtual operator string(){string s=""; return s;};

    protected:
	   string name;
	   int regNo;
};

#endif
