#include "Page.h"

#include <vector>
using namespace std;
#include "TFTDisplay.h"
#include "TimedChunk.h"

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

void Page::update(TFTDisplay* tft){
  for (int i = 0; i < _chunks.size(); i++) {
     if (_chunks[i]->hasUpdate()){
      _chunks[i]->update(tft);
      TimedChunk* cast = static_cast<TimedChunk*>(_chunks[i]);
      if (cast != nullptr){
       cast->markUpdated();
      }
     }
   }
}