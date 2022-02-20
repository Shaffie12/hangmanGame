#ifndef _GAMER_H_
#define _GAMER_H_

#include <string>
#include <iostream>
#include <iomanip>
#include <map>
#include <sstream>
#include "Person.h"

using namespace std;

class NoScoreException: public exception // returns a message to the user if this error is thrown
{
	const char*msg;
	public:
	NoScoreException(const char* s) throw(): msg(s){}
	const char* what() const throw()
	{
		return msg;
	}
};

class Gamer: public Person
{   public:
		Gamer(const string &name, int regNo); //makes a new gamer with a name and regnumber, sets their score map to empty
		int getRegNo() const; //returns gamers reg number
		
		void addScore(float score,string difficulty);
		void setScoresFromFile(); //fills the "scores" multimap with values from the file

		bool updateAccumulativeScore(string difficulty,float score); //updates the accumulative score to reflect all values from file
		void setAccumulatives(); //updates the accumulative score to reflect all values from file
		string printAccumulatives() const; //prints the accumulative scores with correct dp formatting

		bool updateAverageScore(string difficulty,float score); //updates the average score to reflect all values from file
		void setAverages(); //updates the average score to reflect all values from file
		void serialize(ostream& out); //writes a gamer to the users file

		
	  

    private:
	   int regNo;
	   string name;
	   multimap<string,float> scores;  //scores map, with each in range 0.0 to 100.0, and an
	   									//associated level of difficulty 
	   map<string,float> accumulative_scores;  // scores map
	   											//associated level of difficulty 
	   map<string,float> average_scores;  // scores map
	   									//associated level of difficulty 
		
								   
		
    // friend function to output details of gamer to stream
    // outputs name, regno, and minimum, maximum and average score on a single line (if applicable)
    friend ostream& operator<<(ostream &str, const Gamer &s);
	friend bool operator <(const Gamer& a, const Gamer&b); //used to compare the gamers inside a collection
	friend bool operator==(const Gamer& a, const Gamer&b); //used to compare the gamers inside a collection
	
};

#endif
