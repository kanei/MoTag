#include <string>
#include <iostream>
#include <fstream>
#include "taggedData.h"
#include "word.h"

using namespace std;

//konstruktory
taggedData::taggedData(istream* name)
{
  checker = checker->getInstance();
  position=0;
  getCorrectValues=false;
  file = name;
}

//destruktor
taggedData::~taggedData()
{

}

void taggedData::enableCorrectValues()
{
  getCorrectValues=true;
}

void taggedData::disableCorrectValues()
{
  getCorrectValues=false;
}

bool taggedData::valid()
{
  return ((*file).bad()?false:true);
}

//!! MYSLENKA: PREHODIT VSECHNO JEN JAKO UKAZATELE A S NIMI PRACOVAT
//!! UDELAT SI JEDNU STRUKTURU KAM BY SE PRIDAVALY A PAK BY SE NA NE JEN
//!! ODKAZOVALO

//pri uspechu vraci ukazatel na slovo, pri neuspechu NULL
Sentence*taggedData::getSentence(void)
{
  if((*file).bad())    //Kontrola spravneho otevreni souboru
    return NULL;

  Sentence* sentence = new Sentence;
  Word* tempWord;       //objekty na ulozeni aktualniho a predchoziho prvku

  //Nalezeni zacatku vety
  if((*file).good())
  {
    string word;
    do
    {
      getline((*file),word);
    }while(word.find("<s ")==word.npos&&(*file).good());
  }
  else return NULL;

  //nacitani slov dokud nedorazime na konec
  while((tempWord=getWordCSTS())!=NULL)
  {
    position++;
    //pridani slova do vety
    sentence->addWord(*tempWord);
    delete tempWord;
  }
  if(sentence->empty()&&tempWord==NULL)
    return NULL;
  return sentence;
}

//pri uspechu vraci ukazatel na slovo, pri neuspechu NULL
Word*taggedData::getWordCSTS(void)
{
  string word;
  unsigned offset=0;
  unsigned length;
  Word*tempPosib=new Word;
  //nacteni prvniho radku obsahujiciho <f nebo <d a ulozeni do stringu
  if((*file).good())
  {
    unsigned position;
    do
    {
      position = (*file).tellg();
      getline((*file),word);               //pokud narazime na zacatek dalsi vety, vratime se zpet a ukoncime nacitani
      if(word.find("<s ")!=word.npos)
      {
        (*file).seekg(position, ios::beg);
        return NULL;
      }

    }while(word.find("<f ")==word.npos&&word.find("<d ")==word.npos&&(*file).good());
  }
  else return NULL;

  //nastaveni tvaru slova
  offset=word.find(">");
  tempPosib->setForm(word.substr(offset+1,word.find("<",offset+1)-offset-1));
  //std::cout<<word.substr(offset+1,word.find("<",offset+1)-offset-1)<<endl;

  if(getCorrectValues)
  {
    if((offset=word.find("<t>"))!=word.npos)
    {
      //upraveno - pridava proste 15 znaku po <t> nemel by s tim byt problem a je to rychlejsi
      tempPosib->setCorrectTag(word.substr(offset+3,15));//word.find("<",offset)-offset));
    }
    if((offset=word.find("<l>"))!=word.npos)
      tempPosib->setCorrectLemma(word.substr(offset+3,word.find("<",offset+3)-offset-3));
      //std::cout<<word.substr(offset+3,word.find("<",offset+3)-offset-3)<<endl;
    offset=0;
  }
  unsigned offLemma=0;
  unsigned offTag=0;
  //nacteni prvniho lemmatu a zjisteni pozice dalsiho
  if((offLemma=word.find("<MMl ",offLemma))!=word.npos)
  {
    offLemma=word.find(">",offLemma)+1;
    tempPosib->addLemma(word.substr(offLemma,word.find("<",offLemma)-offLemma));
    offLemma=word.find("<MMl ",offLemma);
  }

  //nacteni lemmat do objektu tempPosib
  while((offTag=word.find("<MMt ",offTag))!=word.npos)
  {
    //pri prejiti pozici tagu pres lemma se prida do struktury, cimz se urci ktere znacky nalezely predchozi
    if(offTag>offLemma)
    {
      offLemma=word.find(">",offLemma)+1;
      tempPosib->addLemma(word.substr(offLemma,word.find("<",offLemma)-offLemma));
      offLemma=word.find("<MMl ",offLemma);
    }
    offTag=word.find(">",offTag)+1;
    tempPosib->addTag(word.substr(offTag,word.find("<",offTag)-offTag));

  }

  if(tempPosib->isSet())
    return tempPosib;
  else
  {
    delete tempPosib;
    return NULL;
  }
}
