#include "Gamer.h"
#include "Manager.h"
#include "FileReader.h"
#include "GameInstance.h"
#include "ReadWords.h"
#include <iostream>
#include <vector>
#include <typeinfo>
#include <algorithm>
#include <stdlib.h> //for random int

/*template login function which returns a Person obj
*first search for matching regNo in file and check if it should make a manager or gamer and return that.
*if no match is found with the entered regNo, will create one for user by appending random numbers or making a substring.  Len is always 5 since only gamers can be made during login
*Dup regNo cannot exist.
*/
template<typename S>
Person *login(S& regno) 
{   
    cout<<"logging in..."<<'\n';
    string str_regno = to_string(regno);
    FileReader fr = FileReader("users.txt");
    fr.readUsers();
    
    if(fr.findReg(str_regno))
    {
        string name = fr.getNameAtReg(str_regno);

        if(fr.isMgr(str_regno))
        {
            Manager *currentMgr = new Manager(name,regno);
            return currentMgr; 
        }
        else
        {
           
            Gamer *g = new Gamer(name,regno);
            return g; 
        }
         
    }
    else
    {
        if(str_regno.size()<5){
            srand((unsigned int)time(NULL));
            while(str_regno.size()<5)
            {
                int r = rand()%10;
                str_regno.append(to_string(r));
            }
            
        
        }
        else if(str_regno.size()>5)
        {   
            
            str_regno=str_regno.substr(0,5);
        }

        while(fr.findReg(str_regno))
        {   
            int r = rand() %10;
            str_regno[rand()%5] = r;
        }
        
        
        string name;
        cout<<"You're new here, please enter a name: "<<endl;
        cin.ignore();
        getline(cin,name);
        Gamer *g= new Gamer(name,stoi(str_regno));

        fr.addGamerToMap(*g);

        ofstream out;
        out.open("users.txt",ios_base::app);
        g->serialize(out);
        cout<<endl;
        cout<<"New gamer was added."<<endl;
        cout<<*g;
        return g;
        
    }
     

}

//prints the top 10 options by implementing an iterable vector of Gamers and a reverse iterator to sort
void printT10(Gamer& g)
{
    
    vector<Gamer> allGamers;
    FileReader fr = FileReader("users.txt");
    map<string,string> contents = fr.getFileContents();
    fr.readUsers();
    contents=fr.getFileContents();
    
    map<string,string>::iterator it = contents.begin();
    while(it!=contents.end())
    {
        if(it->first.length()==5)
        {
    
            int regNo = stoi(it->first);
            string name = fr.getNameAtReg(it->first);
            Gamer gamer = Gamer(name,regNo);
            gamer.setScoresFromFile();
            gamer.setAccumulatives();
            gamer.setAverages();
            allGamers.push_back(gamer);
            
        }

        it++;
    }
    
    
    sort(allGamers.rbegin(),allGamers.rend());
    cout<<setw(10)<<left<<"RANK"<<setw(25)<<left<<"PLAYER"<<setw(50)<<left<<"TOTAL SCORE"<<'\n';
    
    vector<Gamer>::iterator it_g=allGamers.begin();
    int i=1;
    while(it_g!=allGamers.end() && i<11)
    {
        if(*it_g==g)
        {
            cout<<setw(10)<<i<<setw(25)<<left<<allGamers[i-1].getName()<<allGamers[i-1].printAccumulatives()<<setw(10)<<" <----YOU"<<'\n';
        }
        else
        {
            cout<<setw(10)<<i<<setw(25)<<left<<allGamers[i-1].getName()<<allGamers[i-1].printAccumulatives()<<'\n';
        }
       it_g++;
       i++;
    }

       
  
    

        
       
       
    
    
    
    
    
    
}

//prints the gamers options
void printGamerMenu()
{
    cout<<"1) Play game"<<endl;
    cout<<"2) Your statistics"<<endl;
    cout<<"3) Display top 10"<<endl;
    cout<<"4) Exit game"<<endl;
}

//prints the managers options
void printManagerMenu()
{
    cout<<"1) Reset game"<<'\n';
    cout<<"2) Change game levels"<<'\n';
    cout<<"3) Add another manager"<<'\n';
    cout<<"4) Set the word file"<<'\n';
    cout<<"5) Exit"<<'\n';
}

//prints the game options and difficulty settings from settings file
void printGameOptions()
{
    FileReader *fr = new FileReader("settings.txt");
    map<string,pair<int,int>>settings = fr->readSettings();
    cout<<"1) Easy -> words with size between "<<settings.find("Easy")->second.first<<"-"<<settings.find("Easy")->second.second<<endl;
    cout<<"2) Medium -> words with size between "<<settings.find("Medium")->second.first<<"-"<<settings.find("Medium")->second.second<<endl;
    cout<<"3) Hard -> words with size "<<settings.find("Hard")->second.first<<" and above"<<endl;
    delete fr;
}

/*runs the program
*calls login function which should return a person, person is casted to 1 of the 2 inherited types and options display according to this cast success.
*/
int main()
{ 
    //container for the gamers and variable to store a manager if they login.
    vector<Gamer> *GAMERS = new vector<Gamer>;
    Manager* CURRENT_MANAGER;

    int option= 0;
    int reg;
    cout<<"Please enter your registration number."<<endl;
    while(!(cin >> reg))
    {
        cout<<"please enter a valid number."<<endl;
        cin.clear();
        cin.ignore();
    }

    Person* logged_in=login(reg);
    //options if person is able to be casted to a gamer
    if(dynamic_cast<Gamer*>(logged_in))
    {   
        Gamer * logged_gamer= dynamic_cast<Gamer*>(logged_in);
        logged_gamer->setScoresFromFile();
        logged_gamer->setAccumulatives();
        logged_gamer->setAverages();
        GAMERS->push_back(*logged_gamer);
        while(option!=4)
        {
            printGamerMenu();
            cin>>option;
            if(option==1)
            {
                int dif;
                cout<<"Please select the mode you wish to play on."<<endl;
                printGameOptions();
                while(!(cin >> dif) || dif >3 || dif <1)
                {

                    cout<<"please enter a valid number."<<endl;
                    cin.clear();
                    cin.ignore();
                    
                }

                GameInstance *inst = new GameInstance(dif, logged_gamer);
                inst->initGame();
                delete inst;
                
            }
            else if(option==2)
            {
                cout<<*logged_gamer;
            }
            else if(option ==3)
            {
                printT10(*logged_gamer);
            }
        }
    }
    else if(dynamic_cast<Manager*>(logged_in))
    {   //options if person is able to be casted to a manager
        CURRENT_MANAGER = dynamic_cast<Manager*>(logged_in);
        while(option!=5)
        {
            printManagerMenu();
            cin>>option;
            if(option==1)
            {
                FileReader fr = FileReader("settings.txt");
                if(fr.resetSettings())
                {
                    cout<<"settings restored to defaults"<<'\n';
                }
                else
                {
                    cout<<"settings could not be restored to default, please check the settings file exists"<<'\n';
                }
            
            }
            else if(option==2)
            {
                int min;
                int max;
                string difficulty;
                
                cout<<"please enter the difficulty you wish to adjust"<<'\n';
                cin>>difficulty;
                FileReader::lowCase(difficulty);
                while(!(difficulty=="easy") && !(difficulty=="medium") && !(difficulty=="hard"))
                {
                    cin.ignore();
                    cout<<"please use: easy, medium or hard."<<endl;
                    cin>>difficulty;
                    FileReader::lowCase(difficulty);

                }
                FileReader::capitalize(difficulty);
                cout<<"please enter the minimum word length"<<endl;
                cin>>min;
                cout<<"please enter the maximum word length"<<endl;
                cin>>max;
                FileReader fr = FileReader("settings.txt");
                fr.changeDifficulty(difficulty,min,max);
               
            }
            else if(option==3) 
            {
                string mgrName;
                int mgrReg;
                cout<<"Please enter a name and registration number for the new manager"<<'\n';
                cin.ignore();
                getline(cin,mgrName);
                while(!(cin >> mgrReg) || mgrReg >9999 || mgrReg <1000)
                {

                    cout<<"please enter a valid number."<<endl;
                    cin.clear();
                    cin.ignore();
                    
                }
                string reg=to_string(mgrReg);
                FileReader f=FileReader("users.txt");
                if(f.addManagerToFile(mgrName,reg))
                {
                    cout<<"Successfully added new manager"<<'\n';
                }
                else
                {
                    cout<<"Could not add manager, try a different registration number"<<'\n';

                }
                
            }
            else if(option==4)
            {
                string wordFile;
                cout<<"please enter the name of the text file to be used"<<endl;
                cin.ignore();
                cin>>wordFile;
                ReadWords::setWordFileName(wordFile);
            }
            
        }
      
        
    }
    else
    {
        cout<<"Please try logging in again.";
        return 0;
    }
    return 0;
    

    
    
        
}