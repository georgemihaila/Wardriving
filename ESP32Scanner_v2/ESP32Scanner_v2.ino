#include "Logger.h"
#include "TFTDisplay.h"
#include "Page.h"
#include "SplashScreen.h"
#include "LEDBlinker.h"

Logger* _logger = new Logger();
TFTDisplay* _display = new TFTDisplay();
SplashScreen* _splashScreen = new SplashScreen();

void setup() {
  _logger->log("ESP32 up");
}

void loop() {
  _display->render(_splashScreen);
}
