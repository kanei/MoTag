#ifndef TAGGEDDATA_H_INCLUDED
#define TAGGEDDATA_H_INCLUDED

#include <string>
#include <fstream>
#include <iostream>
#include <map>
#include "word.h"
#include "sentence.h"
#include "checker.h"

/*
*     Trida implementujici rozhrani ziskavani vet z textu, pro vice info je nutno
*     projit jednotlive hlavickove soubory vety a slova
*     Zatim ziskava data pouze ze souboru formatu CSTS, casem bude doplneno ziskavani prubezne
*     z analyzeru - tim padem z neoznackovaneho textu a dale ze souboru jinych formatu,
*     coz bude rozpoznavat sama trida.
*     prikaz getSentence ziska vetu ze zdroje, ktery ma trida nastaven
 *
 *  Pri startu programu se nejdriv nactou vsechny zname znacky a s nimi jejich jednoznacny
 *  identifikator v podobe unsigned cisla. Pote jsou pri kazdem ukladani dat pro zpracovani
 *  pres hmm vsechny znacky prevedeny na jejich identifikator a takto ulozeny do souboru.
*/

class taggedData{
private:
  std::istream *file;
  int position;                   //pozice v souboru - pocet tagu od zacatku
  Word*getWordCSTS(void);         //ziskani znacky ze souboru typu csts
  bool getCorrectValues;          //urcuje, zda se maji ziskavat spravne znacky
  //!! pri ziskani z jinych typu nutno predelat znacku!!

  Checker* checker;
  //ukladani vsech moznych predchudcu jednotlivych slov. Druhy parametr
  //urcuje slovo, ktere predchazi prvnimu parametru - muze jich byt vic

public:
  taggedData(std::istream* name);          //konstruktor a destruktor pri vytvoreni se ctenim ze souboru
  ~taggedData();

  void enableCorrectValues();     //povoleni/zakazani zisku spravnych moznosti ze vstupu tridy
  void disableCorrectValues();

  bool valid();                   //kontrola spravnosti dat - prevazne otevreni souboru

  Sentence* getSentence(void);    //ziskani vety
};

#endif // TAGGEDDATA_H_INCLUDED
