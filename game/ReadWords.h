/**
 * ReadWords Interface for CE221 assignment 2
 */

#ifndef READWORDS_H
#define READWORDS_H
/**
 * ReadWords class. Provides mechanisms to read a text file, and return
 * lower-case versions of words from that file.
 */
using namespace std;

#include <string>
#include <fstream>
#include <set>
#include <vector>
#include "FileReader.h"

 class ReadWords
 {
   public:
     ReadWords(const char *filename); //opens a file to get words/phrases from

     void close(); //closes the ifstream

    /*cleans (remove spaces and symbols from start/end) and pipes the next available word from text file into the nextWord variable, 
    *sets the eoffound variable to true if calling this function again would be impossible due to EOF
    *returns empty string if word contained numbers or symbols after cleaning
    */
     string getNextWord(); 

     bool isNextWord(); //returns if eof was found

     bool getPhrases(); //gets phrases from the words selected, overloaded version only gets the phrases associated with the argument
     bool getPhrases(const string& word);

    /*gets random words from the file , tries to get 10 words of length specified by the MIN/MAX_WORD_LEN variables but may return false if it could not find 10 of these
    *loops through file from a random position starting point, (between 0 - half the size of the file) and stops when eof is found.
    *so in the worst case scenario, only half the file is searched, works better on larger files.
    */
     bool getWords(); 

    //sets the min and max word lengths for this object to use
     void setMaxWordLen(int& i){MAX_WORD_LEN=i;}
     void setMinWordLen(int& i){MIN_WORD_LEN=i;}
     
     const string sendWord() const; //returns a random word from the set of 10 selected from file
     static char* getWordFileName(); //reads the settings and determines the name of the file being used to generate words
     static bool setWordFileName(const string& fileName); //sets the file to search for words
     
     const set<string> returnPhrases() const{return phrases;} //returns phrases set
     const set<string> getSimilarWords(string& word); //intended to be used with the guessed word in the hangman game.  Takes a string which should be in the format of "_ a _ _ b _ c .." and finds similar words from file
     

   private:
     ifstream wordfile; //ifstream to open/read file to get words from
     string nextword; //holds next word from file
     bool eoffound; //t/f if end of file is found
     set<string> words; //set of 10 words that is filled with readWords
     set<string> phrases; //set of 5 phrases that can be set by getPhrases functions.  Either can get ~50 phrases (5 for each word), or can get 5 (5 for argument word)
     int MAX_WORD_LEN; //min and max word lengths set by difficulty
     int MIN_WORD_LEN;
     long fileSize; //the size of the file being used to get words/phrases
     
 };

 #endif
