/*
 * File:   trainer.cpp
 * Author: Marek
 *
 * Created on 31. kv�ten 2009, 20:54
 */

#include "trainer.h"

Trainer::Trainer(std::istream* name) {
    data = new taggedData(name);
    dictionary = dictionary->getInstance();
    startPos = 0;
    count = 0;
}

bool Trainer::generateMaxEnt(string name)
{
/*  if(!data->valid())
    return false;
  data->enableCorrectValues();

  string modelName(name);
  modelName.append(".model");
  string trainName(name);
  trainName.append(".train");

  ofstream model;
  model.open(modelName.data());
  ofstream train;
  train.open(trainName.data());

  if(model.bad()||train.bad())
    return false;

  Sentence*sentence;
  Word*word;
  stringstream actualForm;

  unsigned previousTag;

  while((sentence = data->getSentence())!=NULL)
  {
    previousForm.clear ();                      //urceni prvniho prvku na prazdny prvek
    previousForm.str ("T|");
    previousForm<<dictionary->nothing;
    previousTag = dictionary->nothing;
    for(unsigned i=0; i<sentence->getWordCount(); i++)  //prochazeni vetou
    {
      word = sentence->getWord(i);


      for(unsigned actTag=0; actTag<word->getTagCount(); actTag++)
      {

        actualForm.clear();
        actualForm.str("");
        actualForm<<"AT"<<previousTag<<"|T"<<word->getTagId(actTag);      //vytvoreni aktualniho prvku

        predecessors.insert(pair<string,string>(previousForm.str(),actualForm.str()));

      }
        actualForm.clear();
        actualForm.str("");
        actualForm<<"AT"<<previousTag<<"|T"<<word->getCorrectTagId();
        previousForm.str(actualForm.str());
        previousTag = word->getCorrectTagId();


    }
  }

  for(multimap<string,string>::iterator item = predecessors.begin(); item!=predecessors.end(); ++item)
  {
    trans<< item->first <<"\t"<< item->second << "\t" << (double)1/predecessors.count(item->first) << endl;

  }

  return true;

*/
}

bool Trainer::generateTransEmit(std::string name)
{
/*  cout << "Viterbi STL example" << endl;

  init_variables();
  print_variables();

  forward_viterbi(observations,
                  states,
                  start_probability,
                  transition_probability,
                  emission_probability);

	cout << "End" << endl;

  string end;
  cin >> end;

  return 0;

*/
  if(!data->valid())             //kontrola spravnosti otevrenych dat
    return false;
  data->enableCorrectValues();

  //otevreni souboru pro vypis dat
  string transName(name);
  transName.append(".trans");
  string emitName(name);
  emitName.append(".emit");
  ofstream trans;
  trans.open(transName.data());
  ofstream emit;
  emit.open(emitName.data());
  ofstream output;
  output.open(name.data());

  //vytvoreni vstupniho souboru
  ofstream input("input");

  if(emit.bad()||trans.bad()||output.bad())
    return false;

  Sentence*sentence;
  Word*word;
  stringstream actualForm;
  stringstream previousForm;
  stringstream saveForm;
  stringstream tmpStr;
  unsigned previousTag;

  stringstream previousSaveForm;

  int counter =0;
  while((sentence = data->getSentence())!=NULL)
  {
    if(counter>=1)
     break;

    previousSaveForm.clear();
    previousSaveForm.str("");

    previousForm.clear ();                      //urceni prvniho prvku na prazdny prvek
    previousForm.str ("");
    previousForm<<"T|"<<dictionary->nothing;
    previousTag = dictionary->nothing;
    for(unsigned i=0; i<sentence->getWordCount(); i++)  //prochazeni vetou
    {
      word = sentence->getWord(i);

      //zapis do vstupnich dat
      output<<"AT"<<previousTag<<"W"<<*word->getForm();
      if(i<sentence->getWordCount()-1)
        output<<" ";


      //for(unsigned actTag=0; actTag<word->getTagCount(); actTag++)
      //{
        //pridani do seznamu nasledovniku
        actualForm.clear();
        actualForm.str("");
        actualForm<<"AT"<<previousTag<<"|T"<<word->getCorrectTagId();//getTagId(actTag);      //vytvoreni aktualniho prvku

        tmpStr.clear();
        tmpStr.str("");
        tmpStr<<previousForm.str()<<"\t"<<actualForm.str();

        predCount[previousForm.str()]++;
        predecessors[tmpStr.str()]++;//.insert(pair<string,string>(previousForm.str(),actualForm.str()));
        //v pripade posledniho slova
        if(i==sentence->getWordCount()-1)
        {
          tmpStr.clear();
          tmpStr.str("");
          tmpStr<<actualForm.str()<<"\t"<<"FINAL";
          predecessors[tmpStr.str()]++;//.insert(pair<string,string>(actualForm.str(),"FINAL"));
          predCount[actualForm.str()]++;
        }

        saveForm.clear();
        saveForm.str("");
        saveForm<<"AT"<<previousTag<<"W"<<*word->getForm();
        //pridani do seznamu pro emit - moznych tagu
        if(previousSaveForm.str() != "")
          possibleTags.insert(pair<string,string>(actualForm.str(), saveForm.str()));
        previousSaveForm.str(saveForm.str());

        emit<<actualForm.str()<<" "<<saveForm.str()<<endl;



        //cout<<actualForm.str()<<" "<<saveForm.str()<<endl;
      //}


      actualForm.clear();
      actualForm.str("");
      actualForm<<"AT"<<previousTag<<"|T"<<word->getCorrectTagId();
      previousForm.str(actualForm.str());

      previousTag = word->getCorrectTagId();


    }

    if(sentence->getWordCount()>0)
    {
      counter++;
      output<<endl;//odradkovani za poslednim slovem vety
    }
  }

  //uvodni stav
  trans<<"T|0"<<endl;

  for(multimap<string,unsigned>::iterator item = predecessors.begin(); item!=predecessors.end(); ++item)
  {
    string tmp = item->first.substr(0,item->first.find("\t"));
    trans<< item->first << "\t" << "0.0227273" <<endl;//(double)1/predCount[tmp]*item->second << endl;

  }

  pair<string,string> previous;

  unsigned start = 1;
  unsigned count;

  for(multimap<string,string>::iterator item = possibleTags.begin(); item!=possibleTags.end(); ++item)
  {
    if(start)//pri prvnim prvku
    {
      start=0;
      previous.first = item->first;
      previous.second = item->second;
      count = 1;
    }
    else  //kontrola, jestli je prvek stejny, pokud ne, vypise se predchozi a novy se vlozi do "zasobniku"
    {
      if(previous.first == item->first && previous.second == item->second)
      {
        count++;
      }
      else
      {
        //vypis a zmena na novy prvek
        //emit<< previous.first <<"\t"<< previous.second <<"\t"<< "0.00001" <<endl;//(double)1/possibleTags.count(previous.first)*count <<endl;
        //cout<< previous.first <<"\t"<< previous.second <<"\t"<< "0.00001" <<endl;//(double)1/possibleTags.count(previous.first)*count <<endl;
        previous.first = item->first;
        previous.second = item->second;
        count = 1;
      }
    }
  }
  //vypsani posledniho prvku
  //emit<< previous.first <<" "<< previous.second <<" "<< "0.00001"<<endl; //(double)1/possibleTags.count(previous.first)*count <<endl;
  //cout<< previous.first <<" "<< previous.second <<" "<< "0.00001"<<endl; //(double)1/possibleTags.count(previous.first)*count <<endl;

  return true;



  /*


    unsigned count = 0;

    Sentence*tmp;
    string*previousCorrectTag;

    bool somethingWritten=false;

    //urceni predchozi znacky jako zacatku vety
    unsigned previous;// = dictionary->strToUns("INIT");
    while((tmp=data->getSentence())!=NULL)
    {
        count++;
        if( count%1000 == 0 )
          cout<<"Precteno "<<count<<"vet."<<endl;
        for(unsigned i=0;i<tmp->getWordCount();i++)
        {
            Word* actWord = tmp->getWord(i);

            //vypsani aktualniho slova do vystupu
            if(!actWord->getFormId()==dictionary->nothing)
              somethingWritten = true;
            input<<*actWord->getForm()<<" ";

            for(unsigned tagN=0; tagN<actWord->getTagCount(); tagN++)
            {
                possibleTags.insert(pair<unsigned, unsigned>(actWord->getTagId(tagN),actWord->getFormId()));
                predecessors.insert(pair<unsigned, unsigned>(previous,actWord->getTagId(tagN)));
            }
            if(actWord->hasCorrectVersion())    //overeni pritomnosti spravne znacky
            {
              //prirazeni soucasne znacky jako predchozi pro dalsi slovo
              previous=actWord->getCorrectTagId();
            }



        //cout<<*tmp->getWord(i)->getLemma(0)<<" tagu:"<<tmp->getWord(i)->getTagCountForLemma(0);
        //for(int j=0;j<tmp->getWord(i)->getTagCount();j++)
        //    cout<<*tmp->getWord(i)->getTag(j)->get();
        //cout<<endl;
        }
        if(somethingWritten)
        {
          input<<endl;
          somethingWritten = false;
        }
        //predecessors.insert(pair<unsigned,unsigned>(previous,dictionary->strToUns("FINAL")));
        //previous = dictionary->strToUns("INIT");

    }
    pair<string,string> prevPred("","");


    for(multimap<unsigned,unsigned>::iterator item = predecessors.begin(); item!=predecessors.end(); ++item)
    {
      //osetreni opakovani prvku
      // TODO udelat jinak

      if(item->first != prevPred.first && item->second != prevPred.second)
        trans<< item->first <<"\t"<< item->second << "\t" << (double)1/predecessors.count(item->first) << endl;
      prevPred.first = item->first;
      prevPred.second = item->second;
    }

    for(multimap<unsigned, unsigned>::iterator item = possibleTags.begin(); item!= possibleTags.end(); ++item)
    {
      if(item->first != prevPred.first && item->second != prevPred.second)
        emit<< item->first <<"\t"<< item->second <<"\t"<< (double)1/10000 <<endl;
      prevPred.first = item->first;
      prevPred.second = item->second;
    }

    string lampa("ahoj");
    lampa = *dictionary->unsToStr(dictionary->strToUns(&lampa));
    cout<<lampa<<endl;
  */
}

void Trainer::init_variables(void) {
  states.push_back("Rainy");
	states.push_back("Sunny");

  observations.push_back("walk");
  observations.push_back("shop");
  observations.push_back("clean");

  start_probability["Rainy"] = 0.6;
  start_probability["Sunny"] = 0.4;

  transition_probability["Rainy"]["Rainy"] = 0.7;
  transition_probability["Rainy"]["Sunny"] = 0.3;
  transition_probability["Sunny"]["Rainy"] = 0.4;
  transition_probability["Sunny"]["Sunny"] = 0.6;

  emission_probability["Rainy"]["walk"] = 0.1;
  emission_probability["Rainy"]["shop"] = 0.4;
  emission_probability["Rainy"]["clean"] = 0.5;
  emission_probability["Sunny"]["walk"] = 0.6;
  emission_probability["Sunny"]["shop"] = 0.3;
  emission_probability["Sunny"]["clean"] = 0.1;
}

void Trainer::print_variables(void) {
  // print states
  cout << "States:" << endl;
  for(vector<string>::iterator i=states.begin();i!=states.end();i++) {
    cout << "S: " << (*i) << endl;
  }
  // print observations
  cout << "Observations:" << endl;
  for(vector<string>::iterator i=observations.begin();i!=observations.end();i++) {
    cout << "O: " << (*i) << endl;
  }

  // print start probabilities
  cout << "Start probabilities:" << endl;
  for(map<string, double>::iterator i=start_probability.begin();i!=start_probability.end();i++) {
    cout << "S: " << (*i).first << " P: " << (*i).second << endl;
  }

  // print transition_probability
  cout << "Transition probabilities:" << endl;
  for(map<string,map<string, double> >::iterator i=transition_probability.begin();i!=transition_probability.end();i++) {
    for(map<string, double>::iterator j=(*i).second.begin();j!=(*i).second.end();j++) {
      cout << "FS: " << (*i).first << " TS: " << (*j).first << " P: " << (*j).second << endl;
    }
  }

    // print emission probabilities
  cout << "Emission probabilities:" << endl;
  for(int i=0; i<states.size(); i++) {
    for(int j=0; j<observations.size(); j++) {
      cout << "FS: " << states[i] << " TO: " << observations[j] <<
        " P: " << emission_probability[states[i]][observations[j]] << endl;
    }
  }
}

void Trainer::forward_viterbi(vector<string> obs, vector<string> states, map<string, double> start_p,
                     map<string, map<string, double> > trans_p,
                     map<string, map<string, double> > emit_p) {
  map<string, Tracking> T;

  for(vector<string>::iterator state=states.begin(); state!=states.end();state++) {
    vector<string> v_pth;
    v_pth.push_back(*state);

    T[*state] = Tracking(start_p[*state], v_pth, start_p[*state]);
  }

  for(vector<string>::iterator output=obs.begin(); output!=obs.end();output++) {
    map<string, Tracking> U;

    for(vector<string>::iterator next_state=states.begin(); next_state!=states.end(); next_state++) {
      Tracking next_tracker;

      for(vector<string>::iterator source_state=states.begin(); source_state!=states.end(); source_state++) {
        Tracking source_tracker = T[*source_state];

        double p = emit_p[*source_state][*output]*trans_p[*source_state][*next_state];
        source_tracker.prob *= p;
        source_tracker.v_prob *= p;

        next_tracker.prob += source_tracker.prob;

        if(source_tracker.v_prob > next_tracker.v_prob) {
          next_tracker.v_path = source_tracker.v_path;
          next_tracker.v_path.push_back(*next_state);
          next_tracker.v_prob = source_tracker.v_prob;
        }
      }

      U[*next_state] = next_tracker;
    }

    T = U;
  }

  // apply sum/max to the final states
  Tracking final_tracker;

  for(vector<string>::iterator state=states.begin(); state!=states.end(); state++) {
    Tracking tracker = T[*state];

    final_tracker.prob += tracker.prob;

    if(tracker.v_prob > final_tracker.v_prob) {
      final_tracker.v_path = tracker.v_path;
      final_tracker.v_prob = tracker.v_prob;
    }
  }

  cout << "Total probability of the observation sequence: " << final_tracker.prob << endl;
  cout << "Probability of the Viterbi path: " << final_tracker.v_prob << endl;
  cout << "The Viterbi path: " << endl;
  for(vector<string>::iterator state=final_tracker.v_path.begin(); state!=final_tracker.v_path.end(); state++) {
    cout << "VState: " << *state << endl;
  }
}


bool Trainer::generateForCRF(void)
{
  if(!data->valid())             //kontrola spravnosti otevrenych dat
    return false;
  data->enableCorrectValues();

  //otevreni souboru pro vypis dat
  ofstream trainData("train.data");
  ofstream testData("test.data");
  ofstream templat("template");

  ostream* output = &cout;

  Word*word;
  Sentence*sentence;

  int counter = 0;
  string*tmp;

  while((sentence = data->getSentence())!=NULL)
  {
    counter++;    //omezeni vypisu
    if(counter<startPos)
    {
      continue;
    }
    else if(counter>startPos + count && count != 0)
    {
      return true;
    }


    for(unsigned i=0; i<sentence->getWordCount(); i++)  //prochazeni vetou
    {
      word = sentence->getWord(i);

      *output<<*word->getForm()<<"\t";             //tvar slova
      for(int j = 0; j< word->getLemmaCount() ;j++)   //vsechna mozna lemmata
      {
        if(j==0)
          *output<<word->getLemmaId(j);
        else
          *output<<"/"<<word->getLemmaId(j);
      }
      *output<<"\t";

      int ok = 1;
      tmp = word->getTag(0);
      char c = tmp->c_str()[0];
      for(int j = 1; j < word->getTagCount(); j++)    //zjisteni, zda maji vsechny znacky stejny POS, pokud ano, zapiseme jej jako feature
      {
        if((*word->getTag(j))[0]!=c)
        {
          ok = 0;
          break;
        }
      }
      if(!ok)
       c = '-';
      *output<<c<<"\t";

      ok = 1;
      c = tmp->c_str()[3];
      for(int j = 1; j < word->getTagCount(); j++)    //zjisteni, zda maji vsechny znacky stejny POS, pokud ano, zapiseme jej jako feature
      {
        if((*word->getTag(j))[0]!=c)
        {
          ok = 0;
          break;
        }
      }
      if(!ok)
       c = '-';
      //*output<<c<<"\t";



      //for( int j = 0; j< 15; j++)          //jednotlive znacky
      //output<<tmp->c_str()[j]<<" ";       //prvni pozice - POS

      /*for(int i = 2; i<15; i++)
      {
        *output<<tmp->c_str()[i];
      }*/

      //*output<<" "<<tmp->c_str()[0];       //prvni pozice - POS
      //*output<<tmp->c_str()[1];       //prvni pozice - POS

      //*output<<endl;
      if(0)
      switch(tmp->c_str()[0])
      {
        case 'A': //adjektivum (přídavné jméno)
          *output<<tmp->c_str()[4];   //pad
          break;
        case 'C': //numerál (číslovka, nebo číselný výraz s číslicemi)
          *output<<tmp->c_str()[4];   //pad
          break;
        case 'D': //adverbium (příslovce)
        case 'I': //interjekce (citoslovce)
        case 'J': //konjunkce (spojka)
          break;
        case 'N': //substantivum (podstatné jméno)
          *output<<tmp->c_str()[4];   //pad
          break;
        case 'P': //pronomen (zájmeno)
          *output<<tmp->c_str()[4];   //pad
          break;
        case 'R': //prepozice (předložka)
        case 'T': //partikule (částice)
          break;
        case 'V': //verbum (sloveso)
          *output<<tmp->c_str()[7];   //osoba
          break;
        case 'X': //neznámý, neurčený, neurčitelný slovní druh
        case 'Z': //interpunkce, hranice věty
          break;
      }
      //*output<<endl;
      //*output<<tmp->c_str()[4]<<" ";       //prvni pozice - POS
      // *output<<tmp->c_str()[8]<<" ";       //prvni pozice - POS

      *output<<*word->getCorrectTag()<<endl;  //to co budeme trenovat - id znacky
    }
    *output<<endl;
  }
  return true;
}
