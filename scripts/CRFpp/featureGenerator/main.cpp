#include <iostream>
#include <string>
#include <cstdlib>
#include "trainer.h"
#include "taggedData.h"
#include "word.h"
#include "taggedData.h"
#include "sentence.h"
#include "featuregenerator.h"

using namespace std;

char help[] = "Program pro konverzi dat ve formatu csts do formatu pouzitelneho \n\
               pro zpracovani skriptem a programem CRF++ \n\
               Data pro zkonvertovani program ocekava na standartnim vstupu a vypis\n\
               je provaden na standartni vystup.\n\
               Program pracuje s nasledujicimi parametry:\n\
               -h       vypise tuto napovedu\n\
               -n X     X znaci pocet zpracovanych prvku\n\
               -s X     X znaci pozici prvniho zpracovaneho prvku";

char shelp[] = "Spatny format parametru, spustte s parametrem -h pro napovedu.\n";

int main(int argc,char* argv[])
{

  //promenne pro urceni parametru programu
  int firstPos = 0;         //index prvniho vypisovaneho prvku
  int count = 0;            //pocet vypisovanych prvku

  /*Mapper*tmp;
  tmp = tmp->getInstance();

  string tmpstr("ahoj");
  unsigned lampa = tmp->strToUns(&tmpstr);
  string* tmplmp = tmp->unsToStr(lampa);
  cout<<*tmplmp<<endl;
*/

  //Zpracovani dalsich parametru programu
  if(argc>2)
  {
     for(int i = 1; i<argc; i++)
     {
        switch(argv[i][1])
        {
        case 'h':
           cout<<help<<endl;
           break;
        case 'n':
           count = atoi(argv[++i]);
           break;
        case 's':
           firstPos = atoi(argv[++i]);
           break;
        }
     }
  }

  if(0)
  {
    featureGenerator*generator;
    generator = generator->getInstance();
    ostream* output = &cout;
    //if(!generator->generate("bin/release/test.csts",output))//text.txt",output))
    {
      fprintf(stderr, "Nepodarilo se vygenerovat features.");
    }
  }

  Trainer train(&cin);
  train.setCount(count);
  train.setStartPos(firstPos);
  if(!train.generateForCRF())
  {
      fprintf(stderr, "Chyba pri nacitani dat");
      return 1;
  }

  /*

  Sentence*tmp;
  //blb[1].setTag("blaf");
  taggedData moznosti("data/train-3-ann.cst");
  moznosti.enableCorrectValues();
  while((tmp=moznosti.getSentence())!=NULL)
  {
    for(int i=0;i<tmp->getWordCount();i++)
    {

        //cout<<*tmp->getWord(i)->getLemma(0)<<" tagu:"<<tmp->getWord(i)->getTagCountForLemma(0);
        //for(int j=0;j<tmp->getWord(i)->getTagCount();j++)
        //    cout<<*tmp->getWord(i)->getTag(j)->get();
        //cout<<endl;
    }

    //cout<<endl;
  }*/

  /*Word blbec;
  blbec.addLemma("blaf");
  blbec.addTag("blb");
  vector<Word> words;
  words.push_back(blbec);*/

  //taggedData blbec("lampa.txt");
  //blbec.getTag();

  return 0;
}
