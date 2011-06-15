#include "featuregenerator.h"

bool featureGenerator::instanceFlag = false;
featureGenerator* featureGenerator::single = NULL;

featureGenerator* featureGenerator::getInstance()
{
    if(!instanceFlag)
    {
      single=new featureGenerator;
      instanceFlag=true;
    }
    return single;
}

featureGenerator::featureGenerator()
{
  dictionary = dictionary->getInstance();
  //ctor
}

featureGenerator::~featureGenerator()
{
  //dtor
}


bool featureGenerator::generate(istream* input, ostream* output)
{
  data = new taggedData(input);
  if(!data->valid())
    return false;
  data->enableCorrectValues();


  Sentence* actSentence;
  stringstream buffer;

  while((actSentence=data->getSentence())!=NULL)
  {
      for(unsigned i=0;i<actSentence->getWordCount();i++)
      {
          Word* actWord = actSentence->getWord(i);

          /*//vypsani aktualniho slova do vystupu
          if(!actWord->getForm()->empty())
            somethingWritten = true;*/

          if(actWord->hasCorrectVersion())    //overeni pritomnosti spravne znacky
          {
            unsigned tmpTagId = actWord->getCorrectTagId();

            //vycisteni stringstreamu
            buffer.clear ();
            buffer.str ("");
            //rys |T - aktualni znacka
            buffer<<"|T"<<tmpTagId;
            //vyhledani a pripadne pridani nebo zvyseni poctu vyskytu rysu
            ++featureStorage[buffer.str()];
            //buffer.seekp(0);

            //rys AT|T - predchozi a aktualni znacka
            if(i>0) //pokud nejsme na 1. znacce
            {
              buffer.clear ();
              buffer.str ("");
              buffer <<"AT"<<actSentence->getWord(i-1)->getCorrectTagId()<<"|T"<<tmpTagId<<"\0";
              ++featureStorage[buffer.str()];
            }
            else  //jinak pridat predchozi prvek jako 0
            {
              buffer.clear ();
              buffer.str ("");
              buffer <<"AT"<<dictionary->nothing<<"|T"<<tmpTagId<<"\0";
              ++featureStorage[buffer.str()];

            }

          }

      }
  }

  //Vypsani jiz upravenych rysu
  for(map<string,unsigned>::iterator i = featureStorage.begin(); i!= featureStorage.end(); i++)
  {
    if(i->second>=MIN_COUNT)
      *output<<i->first<<endl;
  }

  delete data;

  return true;
}
