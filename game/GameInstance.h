
#ifndef _GAME_INST_H_
#define _GAME_INST_H_
#include <string>
#include "ReadWords.h"
#include "FileReader.h"
#include <iomanip>
#include <map>

using namespace std;

//class for running the game itself
class GameInstance{
    private:
    string word; //holds the word to be used for a single game
    bool gameFinished=false; //flags if the game ended or not
    int difficulty; //the difficulty mode the game is being played on
    int MAX_WORD_LEN; //min and max word lengths 
    int MIN_WORD_LEN;
    float gameScore=0; //stores the user score from the game
    Gamer& player; //reference to the gamer obj who initiated the game

    public:
    //constructor, sets the difficulty and player of the game
    GameInstance(int &d, Gamer* g);

    //sets difficulty, sets the words/phrases from the file to be used in the game, and adds user score once game ended
    void initGame();

    //runs the hangman game and returns a float score
    float startGame(const string& word, const set<string> phrases);

    //draws the hangman graphic to the screen
    string drawMan(int w);

    //checks if the game ended or not
    void setGameFinished(bool b){gameFinished=b;}

    //returns the score for a gameInstance obj
    float getGameScore(){return gameScore;}

    //formats the word inside the phrase as the player plays the game
    string generateTip(string phrase, const string& word, string& guessedWord);

    
    


};

#endif