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
  ScanService(WiFiScanner *wifiScanner, BluetoothScanner *bluetoothScanner, GPSService* gpsService, DataManager* dataManager)
  {
    _wifiScanner = wifiScanner;
    _bluetoothScanner = bluetoothScanner;
    _gpsService = gpsService;
    _dataManager = dataManager;

    _wifiScanner->scanAsync();
  };
  void scan();

private:
  int _currentScan = 0;
  WiFiScanner *_wifiScanner;
  BluetoothScanner *_bluetoothScanner;
  GPSService* _gpsService;
  DataManager* _dataManager;
};

#endif
