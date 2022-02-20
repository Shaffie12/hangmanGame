#include "FileReader.h"
#include <sstream>
#include <iomanip>


vector<float> getScoreFromLine(string& s)
{
    vector<float> scores;
    string f;

    for(int i=0;i<s.length();i++)
    {
        if(isdigit(s[i]) || s[i]=='.')
        {
            f+=s[i];
            if(s[i+1]==' '|| i+1==s.length())
            {
               scores.push_back(stof(f));
               f="";
            }

        }
    }
    
    return scores;

}

FileReader::FileReader(const char*fname)
{
    doc.open(fname);
    if(!doc)
    {
        std::cout<<"Could not open file "<<fname<<std::endl;
        exit(1);
    } 
   
}

void FileReader::readUsers()
{   
    fileContents.clear();
    string line;
    while(!doc.eof())
    {
        getline(doc,line);
        
        if (line!="")
        {
            string regno =line.substr(0,line.find(' '));
            string remainder =line.substr(line.find(' ')+1,line.find('\n'));
            fileContents.insert(make_pair(regno,remainder));
        }
    }
    close();
}

map<string,pair<int,int>> FileReader::readSettings()
{
    
    map<string,pair<int,int>> settingsMap;
    string line;
    getline(doc,line);
    while(!doc.eof())
    {   
        getline(doc,line);
        if (line !="")
        {
            string mode = line.substr(0,line.find(' '));
            string::iterator it;
            it=line.begin();
            int i=0;
            int j=0;

            while(*it!=' '){
                j++;
                it++;
            }

            it=line.begin();

            while(*it!=','){
                i++;
                it++;
            }

            int forward = i-j;

            string min = line.substr(line.find(' '),forward);
            string max = line.substr(line.find(',')+1,line.find('\n'));
            if(min=="0")
            {
                min="1";
            }
            if(max=="0")
            {
                max="50";
            }
            const char * minc = min.c_str();
            const char * maxc = max.c_str();

            pair<int,int> minMax = make_pair(atoi(minc),atoi(maxc));
            settingsMap.insert(make_pair(mode,minMax));

        }
        
    }
    close();
    
    return settingsMap;
}

map<string,string>FileReader::addGamerToMap(Gamer& g) //can this throw an error?
{
    string name=g.getName();
    string reg = to_string(g.getRegNo());
    fileContents.insert(make_pair(reg,name));
    return fileContents;  
}

bool FileReader::findReg(const string& id)const 
{return(fileContents.count(id)==1);}

map<string,vector<float>> FileReader::getUserScores(const int& regno)
{

    map<string,vector<float>> playerScores;

    if(fileContents.size()==0)
    {
        FileReader("users.txt");
        readUsers();
    }

    map<string,string>::iterator it = fileContents.begin();
    it=fileContents.find(to_string(regno));

    size_t pos = it->second.find(";")+1;
    string start,e,m,h;
    
    start = it->second.substr(pos);
   
    e = start.substr(0,start.find(','));
    start.erase(0,start.find(',')+1);
   
    m = start.substr(0,start.find('/'));
    start.erase(0,start.find('/')+1);
   
    h = start.substr(0);

    vector<float> easyScores = getScoreFromLine(e);
    playerScores.insert(make_pair("Easy",easyScores));

    vector<float> medScores = getScoreFromLine(m);
    playerScores.insert(make_pair("Medium",medScores));

    vector<float> hardScores = getScoreFromLine(h);
    playerScores.insert(make_pair("Hard",hardScores));

    return playerScores;


}

string FileReader::getNameAtReg(const string& id) const
{
    map<string,string>::const_iterator it = fileContents.find(id);
    string val = it->second;
    return(val.substr(0,val.find(';')));
} 

bool FileReader::isGamer(const string& id) const
{
    return id.size()==5;

}

bool FileReader::isMgr(const string& id) const
{
    return id.size()==4;
}

bool FileReader::writeScoretoFile(const int &id, float score, int difficulty)
{
    if(to_string(id).length()!=5)
    {
        cout<<"cannot update a score for a Manager.  Please login as a gamer."<<endl;
        return false;
    }
    if(fileContents.size()==0)
    {
        FileReader("users.txt");
        readUsers();
    }
    if (score!=0)
    {
        map<string,string>::iterator it;
        it = fileContents.find(to_string(id));
        string playerInfo = it->second;
        
        char delim;
        switch(difficulty)
        {
            case 1:
                delim=';';
                break;    
            case 2:
                delim=',';
                break;
            case 3:
                delim='/';
                break;
        }

        ostringstream stream;
        stream<<fixed<<setprecision(1)<<score;
        string score2dp;
        score2dp = stream.str();
    
        playerInfo.insert(playerInfo.find(delim)+1, score2dp+" ");
        it->second=playerInfo;
        ofstream out;
        out.open("users.txt",ios::trunc);
        it=fileContents.begin();
        while(it!=fileContents.end())
        {
            out<<it->first<<' '<<it->second<<'\n';
            it++;

        }
        out.close();
        return true;  
    }
    return false;  
}
   
bool FileReader::changeDifficulty(string& difficulty, int& min, int& max)
{
    try{
    
        map<string,pair<int,int>> settings = readSettings();
        settings[difficulty].first=min;
        settings[difficulty].second=max;
        map<string,pair<int,int>>::iterator it = settings.begin();

        ofstream out;
        const char * wf=ReadWords::getWordFileName();

        out.open("settings.txt",ios::trunc);
        out<<wf<<'\n';
        it=settings.begin();
        while(it!=settings.end())
        {
            out<<it->first<<' '<<it->second.first<<','<<it->second.second<<'\n';
            it++;

        }
        out.close();
        return true;
        
    }
    catch (const char* ex)
    {
        cout<<"settings could not be updated"<<'\n';
        return false;
    }
    



}

bool FileReader::resetSettings()
{
    try
    {
        ofstream out;
        out.open("settings.txt",ios::trunc);
        out<<"sample.txt"<<'\n';
        out<<"Easy"<<' '<<'1'<<','<<'5'<<'\n';
        out<<"Medium"<<' '<<'6'<<','<<"10"<<'\n';
        out<<"Hard"<<' '<<"10"<<','<<"50";
    
        out.close();
        return true;

    }
    catch (const char* ex)
    {
        cout<<"settings could not be updated"<<'\n';
        return false;
    }
    
}

string FileReader::lowCase(string& s)
{
   string::iterator it;
    it=s.begin();
    while(it!=s.end())
    {
        *it=::tolower(*it);
        it++;
    }

    return s;
    
}

string FileReader::capitalize(string &s)
{
    string::iterator it;
    it=s.begin();
    *it=::toupper(*it);
        
    return s;
}

bool FileReader::addManagerToFile(string& name, string& id)
{
    readUsers();
    if(!findReg(id))
    {
        fileContents.insert(make_pair(id,name+';'));
        map<string,string>::iterator it=fileContents.begin();

        ofstream out;
        out.open("users.txt",ios::trunc);
        while(it!=fileContents.end())
        {
            out<<it->first<<' '<<it->second<<'\n';
            it++;
        }
        close();
        out.close();
        return true;
    }
    else
    {
        return false;
    }
    
}
