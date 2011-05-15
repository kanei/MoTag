#ifndef POSIBILITIES_H_INCLUDED
#define POSIBILITIES_H_INCLUDED

#include <vector>
#include <iostream>
#include "mapper.h"

using namespace std;

/*
*     Trida slova - obsahuuje vsechny mozne znacky plus lemmata a implementuje funkce
*     na jejich vkladani a pripadne odebirani ze struktury.
*     Dale obsahuje parametr urcujici, zda je slovo urceno s/bez spravne moznosti
*
*     Pri ulozeni lemmatu se automaticky nastavi pocitadlo lemmat na teto pozici na 0 a
*     dokud se neulozi dalsi lemma, pri kazdem nacteni znacky se toto pocitadlo inkrementuje.
*     Tim se docili pocitani znacek u jednotlivych lemmat a moznost zjisteni, ktera znacka ke kteremu
*     lemmatu patri.
*
*     Optimalizovano pro praci pomoci identifikatoru pomoci objektu Mapper, pres ktery se zjistuje
*     jake ma retezec cislo a naopak.
*
*/

class Word{
  private:
    vector<unsigned> tags;                       //pole pro ulozeni znacek a lemmat
    vector<unsigned> lemmas;
    vector<int> tagsInLemma;                //vektor pro ulozeni poctu tagu u jednotlivych lemmat - indexy si odpovidaji

    unsigned form;                            //originalni tvar slova

    unsigned correctTag;                         //ukazatele pro ulozeni spravne znacky a lemmatu
    unsigned correctLemma;
    bool includesRightOne;                  //urceni zda obsahuje spravnou moznost

    Mapper*dictionary;                  //slovnik pro prevod z retezce na cislo a zpet

  public:
    Word();
    ~Word();

    unsigned getTagCount();                 //ziska pocet zadanych tagu nebo lemmat
    unsigned getLemmaCount();

    unsigned getTagCountForLemma(int position);  //ziska pocet tagu u lemmy na zadane pozici (od 0)

    bool isSet();                           //zjisti zda ma slovo nastaven alespon jeden tag a lemmu a form

    void setForm(string input);             //nastavi vstupni tvar slova
    string* getForm();                      //ziska tvar slova
    unsigned getFormId();                   //nebo jako jeho id

    void addTag(string input);              //prida moznou znacku
    string* getTag(unsigned position);      //ziska moznou znacku
    unsigned getTagId(unsigned position);   //ziska moznou znacku jako jeji ciselnou hodnotu - rychlejsi
    void removeTag(unsigned position);      //odstrani znacku

    void addLemma(string input);            //prida/ziska/odstrani lemma
    string* getLemma(unsigned position);
    unsigned getLemmaId(unsigned position); //ziska lemma jako jeho id - rychlejsi
    void removeLemma(unsigned position);

    bool hasCorrectVersion();               //zjisti, zda trida slovo obsahuje spravnou moznost

    void setCorrectTag(string input);       //prida/ziska spravnou znacku
    string* getCorrectTag();
    unsigned getCorrectTagId();
    void setCorrectLemma(string input);     //prida/ziska spravne lemma
    string* getCorrectLemma();
    unsigned getCorrectLemmaId();

};

#endif // POSIBILITIES_H_INCLUDED
