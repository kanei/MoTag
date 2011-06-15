#ifndef CHECKER_H
#define CHECKER_H

#include "mapper.h"

/*
*   Trida implementovana jako singleton, ktera se stara o kontrolu znacek
*   V teto tride jsou implementovany funkce ktere overuji, zda je dana znacka
*   napriklad slovesem, v jakem je pade a podobne.
*/

class Checker
{
  private:
    static bool instanceFlag;           //singleton
    static Checker *single;           //ziskani ukazatele na objekt
    Checker();

    void loadDictionary();            //funkce zavolana pri vytvoreni objektu. Stara se o nacteni identifikatoru retezcu ze slovniku

    Mapper* dictionary;               //prevod mezi cislem a retezcem

    unsigned PUNCT;                   //sem se ulozi identifikator punct retezce

  public:
    Checker* getInstance();

    bool isPunctual(unsigned id);       //zjisti, zda ma prvek znacku "Z---------------"
    bool isPunctual(string* input);
};

#endif // CHECKER_H
