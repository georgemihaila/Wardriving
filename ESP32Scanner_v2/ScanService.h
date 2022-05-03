#ifndef ScanService_h_
#define ScanService_h_
#include "WiFiScanner.h"
#include "BluetoothScanner.h"
#include "BluetoothDevice.h"
#include "WiFiNetwork.h"
#include "GPSService.h"
#include <vector>
using namespace std;
#include "DataManager.h"

class ScanService
{
public:
  ScanService(WiFiScanner *wifiScanner, BluetoothScanner *bluetoothScanner, GPSService* gpsService, DataManager* dataManager, void (*autosendFunction)())
  {
    _wifiScanner = wifiScanner;
    _bluetoothScanner = bluetoothScanner;
    _gpsService = gpsService;
    _dataManager = dataManager;
    _autosendFunction = autosendFunction;

    _wifiScanner->scanAsync();
  };
  void scan();
  void setPreviousScanCounts(int wifi, int bt)
  {
    _wifiScanner->chunk->totalNetworks = wifi;
    _bluetoothScanner->chunk->totalNetworks = bt;
  }
private:
  int _currentScan = 0;
  int _lastTotalNumberOfScansCacheTimestamp = 0;
  int _cacheTotalNumberOfScansEvery = 10000; //100,000 writes is 27 hours @1/s
  int _lastCachedAtWifi = 0;
  int _lastCachedAtBT = 0;
  void _cacheTotalNumberOfScans();
  void (*_autosendFunction)();
  bool firstWifiScanCompleted = false;

  WiFiScanner *_wifiScanner;
  BluetoothScanner *_bluetoothScanner;
  GPSService* _gpsService;
  DataManager* _dataManager;
};

#endif
