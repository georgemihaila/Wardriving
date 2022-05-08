#include "SplashScreen.h"
#include "BatteryChunk.h"
#include "GPSChunk.h"
#include <TFT_eSPI.h>
#include "GPSService.h"
#include "WiFiService.h"
#include "WiFiChunk.h"
#include "StatusBarAnimationChunk.h"
#include "WiFiScanner.h"
#include "BluetoothScanner.h"
#include "ThreeWaySwitch.h"
#include "ModeChunk.h"
#include "ScanTypeChunk.h"
#include "FrameTimeChunk.h"

SplashScreen::SplashScreen(GPSService *gpsService, WiFiService *wifiService, WiFiScanner *wifiScanner, BluetoothScanner* bluetoothScanner, ThreeWaySwitch* modeThreeWaySwitch, ThreeWaySwitch* scanTypeThreeWaySwitch)
{
  addChunk(new BatteryChunk(2500));
  addChunk(new WiFiChunk(wifiService, 1000));
  addChunk(new GPSChunk(gpsService, 1000));
  addChunk(new StatusBarAnimationChunk(30));
  addChunk(wifiScanner->chunk);
  addChunk(bluetoothScanner->chunk);
  addChunk(new ModeChunk(modeThreeWaySwitch, 100));
  addChunk(new ScanTypeChunk(scanTypeThreeWaySwitch, 100));
  addChunk(new FrameTimeChunk(1000 / 30));
}