#ifndef Mapper_H
#define Mapper_H

#include <fstream>
#include <map>
#include <vector>
#include <iostream>

/*
*   Objekt implementujici mapovani retezcu na cisla a zpet
*
*/

using namespace std;

class Mapper
{
  private:
    static bool instanceFlag;           //singleton
    static Mapper *single;           //ziskani ukazatele na objekt
    Mapper();

    unsigned lastId;
    map <string,unsigned> dict;          //slovnik pro prevod ze stringu na unsigned
    vector <string> undict;              //pro prevod z unsigned na string

  public:
    Mapper* getInstance();
    ~Mapper();

    unsigned nothing;             //vrati identifikator pro nic

    unsigned strToUns(string* input);     //vrati identifikator pro dany retezec
    string* unsToStr(unsigned input);     //vrati retezec prirazeny k danemu identifikatoru a nebo NULL
};

#endif // Mapper_H
