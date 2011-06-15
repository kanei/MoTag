#include <vector>
#include "word.h"

using namespace std;

Word::Word(){
  dictionary = dictionary->getInstance();

  includesRightOne=false;
  correctLemma=dictionary->nothing;
  correctTag=dictionary->nothing;
  form = dictionary->nothing;
}

Word::~Word(){
  /*if(correctLemma!=NULL)
    delete correctLemma;
  if(correctTag!=NULL)
    delete correctTag;*/
}

bool Word::isSet(){
  return(lemmas.empty()?false:(tags.empty()?false:(form==dictionary->nothing?false:true)));
}

void Word::setForm(string input)
{
  if(!input.empty())
  {
    form = dictionary->strToUns(&input);
  }
}

string* Word::getForm()
{
  return dictionary->unsToStr(form);
}

unsigned Word::getFormId()
{
  return form;
}

/*
      TAGY
*/

unsigned Word::getTagCountForLemma(int position)
{
  return tagsInLemma[position];
}

unsigned Word::getTagCount()
{
  return tags.size();
}

void Word::addTag(string input)
{
  if(!input.empty())
  {
    tagsInLemma[lemmas.size()-1]=tagsInLemma[lemmas.size()-1]+1;     //inkrementovani poctu znacek
    tags.push_back(dictionary->strToUns(&input));      //prevod ze stringu na unsigned
  }
}

string*Word::getTag(unsigned position)
{
  return (position<tags.size()?(dictionary->unsToStr(tags[position])):NULL);
}

unsigned Word::getTagId(unsigned position)
{
  return (position<tags.size()?tags[position]:dictionary->nothing);
}

void Word::removeTag(unsigned position)
{
  if(position<tags.size())
    tags.erase(tags.begin()+position);
}

/*
      LEMMATA
*/

unsigned Word::getLemmaCount()
{
  return lemmas.size();
}

void Word::addLemma(string input)
{
  if(!input.empty())
  {
    tagsInLemma.push_back(0); //pridani pocitadla na konec
    lemmas.push_back(dictionary->strToUns(&input));
  }
}

string*Word::getLemma(unsigned position)
{
  return (position<lemmas.size()?(dictionary->unsToStr(lemmas[position])):NULL);
}

unsigned Word::getLemmaId(unsigned position)
{
  return (position<lemmas.size()?lemmas[position]:dictionary->nothing);
}

void Word::removeLemma(unsigned position)
{
  if(position<lemmas.size())
    lemmas.erase(lemmas.begin()+position);
}


void Word::setCorrectTag(string input)
{
  if(!input.empty())
  {
    correctTag=dictionary->strToUns(&input);
  }
  if(correctLemma!=dictionary->nothing)
    includesRightOne=true;
}

string* Word::getCorrectTag()
{
  if(correctTag!=dictionary->nothing)
  {
    string*tmp = new string(*dictionary->unsToStr(correctTag));
    return tmp;
  }
  else return NULL;
}

unsigned Word::getCorrectTagId()
{
  return correctTag;
}

string* Word::getCorrectLemma()
{
  if(correctTag!=dictionary->nothing)
  {
    string*tmp = new string(*dictionary->unsToStr(correctLemma));
    return tmp;
  }
  else return NULL;
}

unsigned Word::getCorrectLemmaId()
{
  return correctLemma;
}

void Word::setCorrectLemma(string input)
{
  if(!input.empty())
  {
    correctLemma=dictionary->strToUns(&input);
  }
  if(correctTag!=dictionary->nothing)
    includesRightOne=true;
}

bool Word::hasCorrectVersion()
{
  return includesRightOne;
}

