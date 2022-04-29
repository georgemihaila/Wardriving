#include "Page.h"

#include <vector>
using namespace std;
#include <TFT_eSPI.h>

void Page::addChunk(Chunk* chunk){
  _chunks.push_back(chunk);
}

bool Page::hasUpdate(){
   for (int i = 0; i < _chunks.size(); i++) {
     if (_chunks[i]->hasUpdate()){
       return true;
     }
   }
   return false;
}

void Page::update(TFT_eSPI* tft){
  for (int i = 0; i < _chunks.size(); i++) {
     if (_chunks[i]->hasUpdate()){
       _chunks[i]->update(tft);
     }
   }
}