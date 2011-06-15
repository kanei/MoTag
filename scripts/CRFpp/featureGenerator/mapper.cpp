#include "mapper.h"

bool Mapper::instanceFlag = false;
Mapper* Mapper::single = NULL;

Mapper* Mapper::getInstance()
{
    if(!instanceFlag)
    {
      single=new Mapper;
      instanceFlag=true;
    }
    return single;
}

Mapper::Mapper()
{
  //Nacteni dat z disku
  ifstream inputFile("Mapper.dct");

  nothing = 0;

  if(inputFile.is_open())
  {
    unsigned tmpUns;
    string tmpStr;
    //nacteni dat ze souboru do mapovaci struktury
    while(inputFile.good())
    {
      inputFile>>tmpStr>>tmpUns;
      dict.insert(pair<string,unsigned>(tmpStr,tmpUns));
      undict.push_back(tmpStr);
    }
    inputFile.close();

    lastId = tmpUns;
  }
  else
  {

    dict.insert(pair<string,unsigned>("NOTHING",0));
    dict.insert(pair<string,unsigned>("Z:-------------",1));
    undict.push_back("NOTHING");
    undict.push_back("Z:-------------");
    lastId = 1;
  }

  return;
}

Mapper::~Mapper()
{
  //ulozeni dat do souboru
  ofstream outputFile("Mapper.dct");
  if(outputFile.is_open())
  {
    for(map<string,unsigned>::iterator it=dict.begin(); it!=dict.end(); ++it)
    {
      outputFile<<it->first<<" "<<it->second<<endl;
    }
    outputFile.close();
  }
}

unsigned Mapper::strToUns(string* input)
{
  //pokud neni dosud obsazen
  if(!dict.count(*input))
  {
    dict.insert(pair<string,unsigned>(*input,++lastId));
    undict.push_back(*input);
    return lastId;
  }
  else
  {
    return dict[*input];
  }
}

string* Mapper::unsToStr(unsigned input)
{
  //std::cout<<undict[input-1];
  return &undict[input];
}
