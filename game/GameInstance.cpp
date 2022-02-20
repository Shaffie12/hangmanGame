#include "GameInstance.h"


void printGuess(string& s)
{
    for (char c:s)
    {
        cout<<setw(6)<<left<<c;
    }
    cout<<endl;
}

string GameInstance::generateTip(string phrase, const string& word, string& guessedWord)
{

    phrase.replace(phrase.find(word),word.size(),guessedWord);
    return phrase;
}


GameInstance::GameInstance(int& d, Gamer* g):player(*g)
{
    difficulty=d;
}

void GameInstance::initGame()
{   
    FileReader fr = FileReader("settings.txt");
    FileReader fr2 = FileReader("users.txt");
    const char* fileName = ReadWords::getWordFileName();
    ReadWords rw = ReadWords(fileName);
    delete fileName;

    string difStr;
    switch(difficulty)
    {
        case 1:
            difStr="Easy";
            break;
        case 2:
            difStr="Medium";
            break;
        case 3:
            difStr="Hard";
            break;
    }
        

    map<string,pair<int,int>>settings =fr.readSettings();
    fr.close();

    rw.setMaxWordLen(settings.find(difStr)->second.second);
    rw.setMinWordLen(settings.find(difStr)->second.first);
    
    set<string> phrases;
    string gameWord;

    if(rw.getWords())
    {
        gameWord =rw.sendWord();
        FileReader::lowCase(gameWord);
        
        rw.getPhrases(gameWord);
        phrases = rw.returnPhrases();
        if(phrases.size()<5)
        {
            cout<<"5 phrases for this word could not be found, starting game with limited tips"<<endl;
        }
        rw.close();
        
        gameScore = startGame(gameWord, phrases);
        fr2.writeScoretoFile(player.getRegNo(),gameScore,difficulty);
        player.addScore(gameScore,difStr);
        player.updateAccumulativeScore(difStr,gameScore);
        player.updateAverageScore(difStr,gameScore);
    }
    
    
}

string GameInstance::drawMan(int w)
{
    string man ="---o-I-<";
    string currentMan="";
    for(int i=0;i<w;i++)
    {
        currentMan+=man[i];
    }

    return currentMan;
}

float GameInstance::startGame(const string& word, const set<string> phrases)
{
    set<string>::iterator sit=phrases.begin();
    int correctGuesses=0;
    int wrongGuesses=0;
    float score=0;
    map<int, char> charsInWord;
    string guessed;

    for(int i=0; i<word.length();i++)
    {
        guessed+='_';
        charsInWord.insert(make_pair(i,word[i]));
    }
    cout<<generateTip(*sit,word,guessed)<<endl;
    printGuess(guessed);
    cout<<endl;
    
    map<int,char>::iterator it;
    char g;
    while(!gameFinished)
    {   
        
        cout<<"Guess a letter"<<endl;
        cin.ignore();
        cin>>g;
        //if we havent  guessed this letter and its in the word
        if(guessed.find(g) == string::npos && word.find(g)!=string::npos)
        {
            it=charsInWord.begin();
            while(it!=charsInWord.end())
            {
                if(it->second==g)
                {
                    guessed[it->first]=g;
                    correctGuesses++;
                    switch (difficulty){
                        case 1:
                            score+=1;
                            break;
                        case 2:
                            score+=2;
                            break;
                        case 3:
                            score+=3;
                            break;

                    }
                    if (correctGuesses==charsInWord.size())
                    {
                        cout<<endl;
                        cout<<"congratulations! You finished with score: "<<score<<endl;
                        gameFinished=true;
                    }
                    
                }
                it++;
            }
           
        }
        //if we havent seen it before and its not in the word
        else if(guessed.find(g) == string::npos && word.find(g)==string::npos)
        {
            if(score>0)
            {
                score-1;
            }
            wrongGuesses++; 
        } 
        cout<<endl;
        cout<<drawMan(wrongGuesses)<<endl;
        if(drawMan(wrongGuesses)=="---o-I-<")
        {
            cout<<endl;
            cout<<"GAME OVER"<<endl;
            gameFinished=true;
            cout<<"you finished with score: "<<score<<'\n';
        }
        
        cout<<generateTip(*sit,word,guessed)<<endl;
        printGuess(guessed);
        cout<<endl;

        if(wrongGuesses>=5 && !gameFinished)
        {
            const char* fileName = ReadWords::getWordFileName();
            ReadWords rw = ReadWords(fileName);
            set<string> similarWords = rw.getSimilarWords(guessed);
            if(similarWords.size()>0)
            {   
                int i=0;
                set<string>::iterator s_it=similarWords.begin();
                cout<<'[';
                while(s_it!=similarWords.end() && i<=5) 
                {
                    cout<<*s_it<<" , ";
                    s_it++;
                    i++;
                }
                cout<<']'<<'\n';
            }
            
        }

    }

    return score;
    

}