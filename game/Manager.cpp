#include "Manager.h"

Manager::Manager(const string &name, int regNo):Person(name){
    this->name=name;
    this->regNo=regNo;
}

int Manager::getRegNo()const{return regNo;}
void Manager::serialize(ostream & out){
    out<<this->regNo<<" "<<this->name<<";"<<"\n";

}