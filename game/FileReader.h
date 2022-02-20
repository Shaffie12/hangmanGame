#ifndef _FILEREADER_H_
#define _FILEREADER_H_
using namespace std;
#include "Gamer.h"
#include "Manager.h"
#include <fstream>
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include"ReadWords.h"

class FileReader{

    private:
    ifstream doc; //stream to read a document with
    map<string,string>fileContents; //holds the contents of a file (usually used with users.txt)

    public:
    FileReader(const char*fname);
    void readUsers(); //reads users into the map member
    void close(){doc.close();} //closes the ifstream

    //adds a gamer/manager to the file by adding them to file contents and writing that to file
    map<string,string> addGamerToMap(Gamer& g); 
    bool addManagerToFile(string& name, string& id);
    
    map<string,pair<int,int>> readSettings(); //returns a map of the difficulty settings from the settings file

    map<string,string> getFileContents() const {return fileContents;} //returns the map containing the file contents
    bool findReg(const string& id) const; //determines if the reg number exists in the file already or not
    string getNameAtReg(const string& id) const; //gets the name of the user with a registered id/regno
    map<string,vector<float>> getUserScores(const int& regno); //gets the scores of a user specified by their id

    //determine if an id is for a gamer or manager
    bool isGamer(const string& id) const;
    bool isMgr(const string& id) const;
    
    bool writeScoretoFile(const int &id, float score, int difficulty); //writes a user score to the appropriate position in the file 
    bool changeDifficulty(string& difficulty,int& min, int& max); //writes new values to the settings file for difficulty of each mode
    bool resetSettings(); //resets the settings to default
    
    //static methods to be used for general purposes
    static string lowCase(string &s);
    static string capitalize(string &s);
    

    

};
#endif


