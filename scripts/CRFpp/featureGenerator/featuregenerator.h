#ifndef FEATUREGENERATOR_H
#define FEATUREGENERATOR_H

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include "taggedData.h"
#include "sentence.h"
#include "mapper.h"

using namespace std;

class featureGenerator
{
  private:
    static bool instanceFlag;           //singleton
    static featureGenerator *single;           //ziskani ukazatele na objekt
    featureGenerator();
    taggedData*data;                    //oznackovana data k nacteni
    Mapper*dictionary;                  //slovnik pro prevod ze slova na cislo a zpet
    static const unsigned MIN_COUNT = 1;       //urcuje minimalni pocet vyskytu rysu pro jeho vypsani
    map <string,unsigned> featureStorage;//sem se ukladaji rysy a jejich pocet


  public:
    featureGenerator* getInstance();
    ~featureGenerator();
    bool generate(istream* input , ostream* output);     //generovani features z input do vystupniho datoveho proudu output

};

#endif // FEATUREGENERATOR_H
