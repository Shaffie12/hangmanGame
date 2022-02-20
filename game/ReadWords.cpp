// replace my comments with your own
// except: you can use my comment about one-word lookahead

// you may modify the code that I have supplied if you wish


using namespace std;

#include "ReadWords.h"
#include <cctype>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <algorithm>

//gets the size of the file in bytes, used to generate a random start point between the start and mid point
long getFileSize(const char*& fname)
{
  int size;
  ifstream stream(fname,ios::binary);
  stream.seekg(0,ios::end);
  size = stream.tellg();
  return size;

}

//removes whitespaces and symbols from the start and end of the word
string removePunct(string& word)
{ 
  word.erase(std::remove(word.begin(), word.end(),' '), word.end());
  int i=0;
  int b= word.size()-1;

  while(ispunct(word[i]))
  {
    word.erase(i,1);
    i++;
  }
  while(ispunct(word[b]))
  {
    word.erase(b,1);
    b--;
  }
  
  for(int j=0; j<word.size();j++)
  {
    word[j]=tolower(word[j]);
  }

  return word;
}

ReadWords::ReadWords(const char *fname)
{   
  wordfile.open(fname);
//open file
    if (!wordfile)
    {   cout << "Failed to open " << fname << endl;
        exit(1);
    }
    fileSize=getFileSize(fname);
    //wordfile >> nextword;
    eoffound = false;
}

string ReadWords::getNextWord()
{ 
  wordfile >> nextword;
  string word = nextword;
  word = removePunct(word);
  

  if (wordfile.eof())
  {
    eoffound = true;
  }
  if(word.size()==0){
    return "";
  }

  for(char c: word){
    if(isdigit(c) || !isalpha(c)){
      return "";
    }
  }

  return word;
}

bool ReadWords::isNextWord(){ return !eoffound;}

bool ReadWords::getPhrases()
{
  if (words.size()!=0)
  {
    set<string>::iterator it=words.begin();
    
    
    string line;

    while(it!=words.end())
    {
      wordfile.seekg(0,ios::beg);
      int i=0;

      while(i<5)
      {
        if(!getline(wordfile,line))
        {
          cout<<"could not find 5 phrases for word"<<endl;
          wordfile.clear();
          break;
        }
        if(line.find(' '+*it+' ')!=string::npos)
        {
          phrases.insert(line);
          i++;

        }
        
      }

      it++;

    }

    
    return true;

  }
  
  return false;
}

bool ReadWords::getPhrases(const string& word) 
{
  phrases.clear();
  string line;
  wordfile.clear();
  wordfile.seekg(0,ios::beg);
  int i=0;
  while(i<=5)
  {
    if(!getline(wordfile,line))
    {
      return false;
    }
    else
    {
      FileReader::lowCase(line); 
      if(line.find(word)!=string::npos)
      {
        if(line.find(word)==0 || line.find(word)==line.size()-word.size())
        {
          phrases.insert(line);
          i++;
        }
        else if(!isalnum(line.at(line.find(word)-1)) && !isalnum(line.at(line.find(word)+word.size())) )
        {
          phrases.insert(line);
          i++;
        }
      

      }
    } 
  }
  
  cout<<"successfully found 5 phrases"<<endl;
  return true;

}

//put the file pointer in a random position, read forward until a space, then get the next word
//if the word is not acceptable, keep going forward until an acceptable word is found, if EOF, reset to start, go forward until we reached position where we started.  
//Print error and return false if could not find words after looping.
//if acceptable words are found, they are put into the set until the set size is 5;
bool ReadWords::getWords()
{   
  words.clear();
  srand((unsigned int)time(NULL));
  string ptr;
  long shift=rand()%fileSize/2;
  wordfile.seekg(shift,std::ios::beg);
  char c = wordfile.get();
  while(c!=' ')
    {
      wordfile.get(c);
    }

  //this only searches half the word file on average
  while(words.size()<10)
  {
    ptr=getNextWord();

    while(ptr=="" || !(ptr.size()>=MIN_WORD_LEN && ptr.size()<=MAX_WORD_LEN))
    {
      ptr=getNextWord();
      if(eoffound)
      {
        cout<<"found end of file but could not find 10 suitable words for game, please have manager adjust the word file."<<endl;
        return false;
      }
    }
    words.insert(ptr);
  }
  return true;
}

const string ReadWords::sendWord() const
{
  srand((unsigned int)time(NULL));
  long rndm =rand()%10;
  set<string>::iterator it = words.begin();
  for(int i=0;i<rndm;i++)
  {
    it++;
  }
  return *it;
}

void ReadWords::close(){wordfile.close();}

char* ReadWords::getWordFileName()
{
  ifstream file;
  file.open("settings.txt");
  string fileName;
  getline(file,fileName);
  file.close();


  char* wordFileName = new char[fileName.size()];
  for(int i=0;i<fileName.size();i++)
  {
    wordFileName[i]=fileName.at(i);
    
  }
  wordFileName[fileName.size()]='\0';
  return wordFileName;
  
} 

bool ReadWords::setWordFileName(const string& fileName)
{
  vector<string> lines;
  ifstream s;
  s.open("settings.txt");
  if(!s)
  {   
    cout << "Failed to open file"<<endl;
    return false;
  }
  else
  {
    string line;
    while(!s.eof())
    {
      getline(s,line);
      lines.push_back(line);
    } 
    s.close(); 
    cout<<"changed: "<<lines.at(0)<<endl;
    lines.at(0)=fileName;
    cout<<"to: "<<lines.at(0)<<endl;

    ofstream out;
    out.open("settings.txt",ios::trunc);
    vector<string>::iterator it=lines.begin();
    while(it!=lines.end())
    {
      if (*it!="")
      {
        out<<*it<<'\n';
        
      }
      it++;
    }

    out.close();

    return true;

  }



}

const set<string> ReadWords::getSimilarWords(string& word)
{
  set<string> similarWords;
  string aMatch;
  //make a map of the word that is passed in but only include characters that are alphabetical
  map<int, char> charsInWord;
  map<int,char>::iterator it;
  for(int i=0; i<word.size();i++)
  {
    if(isalpha(word[i]))
    {
      charsInWord.insert(make_pair(i,word[i]));
    }
    
  }

  while(!eoffound)
  {
    aMatch=getNextWord();

    if(aMatch.size()==word.size())
    {
      bool fullMatch=true;

      //make a map of the word we just got pos -> char
      map<int,char> matchMap;
      for(int i=0; i<aMatch.size();i++)
      {
       matchMap.insert(make_pair(i,aMatch[i]));
      }

      map<int,char>::iterator it2 =matchMap.begin();
      it=charsInWord.begin();
      //loop over the characters user has guessed, look for them at the same position in the word we just found. If any characters are found to not match, break and start again with new word
      while(it!=charsInWord.end())
      {
        it2=matchMap.find(it->first);
        if(it->second !=it2->second)
        {
          fullMatch=false;
          break;
        }
        it++;
      }
      if(fullMatch)
      {
        //the function also doesnt know what word it truly is, because its not passed in
        similarWords.insert(aMatch);
        
      }
      
    }
  }

  return similarWords;
  
}

