#include <vector>
#include "sentence.h"

using namespace std;

bool Sentence::empty(){
  return(words.size()>0?false:true);
}

unsigned Sentence::getWordCount(){
  return words.size();
}
void Sentence::addWord(Word input){
  if(input.isSet())
    words.push_back(input);
}
Word* Sentence::getWord(int position){

  return position<words.size()?&(words[position]):NULL;
}
