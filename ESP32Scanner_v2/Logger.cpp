#include "Arduino.h"
#include "Logger.h"

Logger::Logger(){
  Serial.begin(115200);
}

void Logger::log(String message){
  Serial.println(message);
}