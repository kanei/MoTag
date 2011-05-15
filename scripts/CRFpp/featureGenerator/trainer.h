/*
 * File:   trainer.h
 * Author: Marek
 *
 * Created on 31. kv�ten 2009, 20:54
 *
 * implementuje tridu Trainer, ktera se stara o natrenovani ze souboru
 * urceneho jedinym parametrem konstruktoru, ktery je povinny.
 */
#include <fstream>
#include <map>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include "mapper.h"
#include "taggedData.h"
#include "sentence.h"
#include "word.h"

#ifndef _TRAINER_H
#define	_TRAINER_H

using namespace std;

class Tracking {
public:
  double prob;
  vector<string> v_path;
  double v_prob;

  Tracking() {
    prob = 0.0;
    v_prob = 0.0;
  }

  Tracking(double p, vector<string> & v_pth, double v_p) {
    prob = p;
    v_path = v_pth;
    v_prob = v_p;
  }
};

class Trainer {
private:
    Mapper* dictionary;                     //prevod na cisla a zpet
    taggedData* data;
    map<string,unsigned> predecessors;   //prvni prvek urcuje, o ktery tag se jedna. Druhy prvek pak urcuje
                            //vsechny jeho predchudce.
    map<string,unsigned> predCount;     //pocita pocet predchudcu se stejnym tvarem pro spravny vypis
    multimap<string,string> possibleTags;   //pridava do struktury vsechny mozne znacky, ktere dane slovo muze mit

    unsigned startPos;
    unsigned count;
    //VITERBI
    vector<string> states;
    vector<string> observations;
    map<string,double> start_probability;
    map<string,map<string, double> > transition_probability;
    map<string,map<string, double> > emission_probability;

    void init_variables(void);
    void print_variables(void);
    void forward_viterbi(vector<string> obs, vector<string> states, map<string, double> start_p,
                    map<string, map<string, double> > trans_p,
                    map<string, map<string, double> > emit_p);



public:
    Trainer(std::istream* name);          //konstruktor a destruktor pri vytvoreni se ctenim ze souboru

    void setStartPos(unsigned val)        //nastavi startovaci pozici pro vypis
      {startPos = val;}
    void setCount(unsigned val)
      {count = val;}

    //bool generateNextTag(Sentence* sentence, unsigned word, unsigned tag)

    //vytvoreni souboru pro pozdejsi zpracovani pomoci hmm
    bool generateTransEmit(std::string name);
    bool generateMaxEnt(string name);

    bool generateForCRF(void);


};

#endif	/* _TRAINER_H */

