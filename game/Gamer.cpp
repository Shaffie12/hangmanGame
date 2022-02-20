#include "Gamer.h"
#include "FileReader.h"

Gamer::Gamer(const string &name, int regNo): Person(name)
{
    this->name=name;
    this->regNo=regNo;
    scores.clear();
}


int Gamer::getRegNo() const 
{
    return regNo;
}

void Gamer::setScoresFromFile()
{
    FileReader fr=FileReader("users.txt");
    map<string,vector<float>> userScores = fr.getUserScores(regNo);

    scores.clear();
    map<string,vector<float>>::iterator it=userScores.begin();
    while(it!=userScores.end())
    {
        for(float f: it->second)
        {
           addScore(f,it->first); 
        }
        it++;
    }

}

void Gamer::addScore(float score,string difficulty)
{
    scores.insert(pair<string,float>(difficulty,score));
}

void Gamer::setAccumulatives()
{
    FileReader fr = FileReader("users.txt");
    map<string,vector<float>> userScores = fr.getUserScores(regNo);
    map<string,vector<float>>::iterator it;
    it=userScores.begin();
    float t=0;
    while(it!=userScores.end())
    {   
        for(float f:it->second)
        {
            t+=f;
        }
        accumulative_scores.insert(make_pair(it->first,t));
        t=0;
        it++;
    } 

}

bool Gamer::updateAccumulativeScore(string difficulty,float score) 
{
    try
    {
        map<string,float>::iterator it;
        it=accumulative_scores.find(difficulty);
        if (score==0)
        {
            throw NoScoreException("Accumulative score was not updated due to score 0");
        }
        else
        {
            it->second+=score;
            return true;
        }

        
    }
    catch(NoScoreException e)
    {
        cout<<e.what()<<'\n';
        return false;
    }


    
}

void Gamer::setAverages()
{
    average_scores.clear();
    FileReader fr("users.txt");
    map<string,vector<float>> userScores = fr.getUserScores(regNo);
    map<string,vector<float>>::iterator it;

    it=userScores.begin();
    float t=0;
    while(it!=userScores.end())
    {   
        for(float f:it->second)
        {
            t+=f;
        }
        t=t/it->second.size();
        
        average_scores.insert(make_pair(it->first,t));
        t=0;
        it++;
    }
  

}

bool Gamer::updateAverageScore(string difficulty, float score)
{
    try
    {
        map<string,float>::iterator it;
        it=average_scores.find(difficulty);
        if (score==0)
        {
            throw NoScoreException("Average score was not updated due to score 0");
        }
        else
        {
            setAverages();
            return true;
        }

        
    }
    catch(NoScoreException e)
    {
        cout<<e.what()<<'\n';
        return false;
    }
}

void Gamer::serialize(ostream & out){
    out<<this->regNo<<" "<<this->name<<";"<<","<<"/"<<"\n";

}

ostream& operator<<(ostream &str, const Gamer &s)
{
    str<<s.name<<" "<<to_string(s.regNo)<<":"<<'\n'<<'\n';
    if(s.scores.empty())
    {
        str<<"no scores to display"<<'\n';
    }
    else
    {
        multimap<string,float>::const_iterator mmit = s.scores.begin();
        map<string,float>::const_iterator it=s.average_scores.find("Easy");

        str<<"ALL SCORES: ";
        str<<fixed<<setprecision(1);
        while(mmit!=s.scores.end())
        {
            str<<mmit->second<<" ";
            mmit++;

        }
        str<<'\n'<<'\n';

        str<<"AVERAGE SCORES: ";
        str<<it->first<<": "<<it->second<<'\t'<<'\t';

        it=s.average_scores.find("Medium");
        str<<it->first<<": "<<it->second<<'\t'<<'\t';

        it=s.average_scores.find("Hard");
        str<<it->first<<": "<<it->second<<'\t'<<'\t';
        str<<'\n'<<'\n';

        str<<"ACCUMULATIVE SCORES: ";
        it=s.accumulative_scores.find("Easy");
        str<<it->first<<": "<<it->second<<'\t'<<'\t';

        it=s.accumulative_scores.find("Medium");
        str<<it->first<<": "<<it->second<<'\t'<<'\t';

        it=s.accumulative_scores.find("Hard");
        str<<it->first<<": "<<it->second<<'\t'<<'\t';
        str<<'\n'<<'\n';

    }
    
    
    return str;
    
}

bool operator <(const Gamer& a, const Gamer&b)

{
    map<string,float> aAccums = a.accumulative_scores;
    map<string,float> bAccums = b.accumulative_scores;

    map<string,float>::iterator it=aAccums.find("Hard");
    map<string,float>::iterator it2=bAccums.find("Hard");

    if(it->second < it2->second)
    {
        return true;
    }
    else if(it->second > it2->second)
    {
        return false;
    }
    else
    {
        it=aAccums.find("Medium");
        it2=bAccums.find("Medium");
        if(it->second < it2->second)
        {
            return true;
        }
        else if(it->second > it2->second)
        {
            return false;
        }
        else
        {
            it=aAccums.find("Easy");
            it2=bAccums.find("Easy");
            if(it->second < it2->second)
            {
                return true;
            }
            else if(it->second > it2 ->second)
            {
                return false;
            }
            else
            {
                return false;
            }
        }

    }

    
}

string Gamer::printAccumulatives()  const
{

    ostringstream stream;
    string s;
    string score2dp;
    map<string,float>::const_iterator it;
    it=accumulative_scores.begin();
    
    it=accumulative_scores.find("Hard");
    stream<<fixed<<setprecision(1)<<it->second;
    score2dp = stream.str();
    s+="Hard: "+score2dp+ '\t';
    stream.str("");

    it=accumulative_scores.find("Medium");
    stream<<fixed<<setprecision(1)<<it->second;
    score2dp = stream.str();
    s+="Medium: "+score2dp+'\t';
    stream.str("");

    it=accumulative_scores.find("Easy");
    stream<<fixed<<setprecision(1)<<it->second;
    score2dp = stream.str();
    s+="Easy: "+score2dp;
    

    return s;

}

bool operator==(const Gamer& a, const Gamer&b)
{
    if(a.getRegNo()==b.getRegNo())
    {
        return true;
    }
    return false;
}