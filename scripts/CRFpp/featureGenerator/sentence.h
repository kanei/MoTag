#ifndef SENTENCE_H_INCLUDED
#define SENTENCE_H_INCLUDED

#include <vector>
#include <string>
#include "word.h"

using namespace std;

/*
*   Trida definujici vetu - pole nekolika slov oddelenych oddelovacimi znackami
*   v tomto pripade ve formatu Z---------
*   Impplementuje rozgrani pro pridani slova, jeho ziskani a ziskani poctu slov
*/

class Sentence{
  private:
    vector<Word> words;                 //Pole obsahujici jednotliva slova
  public:
    bool empty();                       //funcke zjistujici zda je veta prazdna
    unsigned getWordCount();            //ziskani poctu slov ve vete
    void addWord(Word input);           //pridani/ziskani slova z vety
    Word* getWord(int position);
};

#endif // SENTENCE_H_INCLUDED
