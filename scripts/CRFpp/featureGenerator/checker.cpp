#include "checker.h"

bool Checker::instanceFlag = false;
Checker* Checker::single = NULL;

Checker* Checker::getInstance()
{
    if(!instanceFlag)
    {
      single=new Checker;
      instanceFlag=true;
    }
    return single;
}

//TODO Zkusit udelat buffer pro urychleni kontroly

Checker::Checker()
{
  dictionary = dictionary->getInstance();
  loadDictionary();
  //ctor
}

void Checker::loadDictionary()
{
  string tmp("Z:-------------");
  PUNCT = dictionary->strToUns(&tmp);
}

/*
*   overovaci funkce
*/

bool Checker::isPunctual(unsigned id)
{
  return(id == PUNCT?true:false);
}

bool Checker::isPunctual(string* input)
{
  string* tmp = input;
  return(dictionary->strToUns(input) == PUNCT ?true:false);
}
