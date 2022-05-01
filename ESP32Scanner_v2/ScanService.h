#ifndef ScanService_h_
#define ScanService_h_
#include "WiFiScanner.h"
#include "BluetoothScanner.h"
#include "BluetoothDevice.h"
#include "WiFiNetwork.h"
#include <vector>
using namespace std;

class ScanService
{
public:
  ScanService(WiFiScanner *wifiScanner, BluetoothScanner *bluetoothScanner)
  {
    _wifiScanner = wifiScanner;
    _bluetoothScanner = bluetoothScanner;

    _wifiScanner->scanAsync();
  };
  void scan();

private:
  void autosend(vector<WiFiNetwork> networks);
  void autosend(vector<BluetoothDevice> devices);
  int _currentScan = 0;
  WiFiScanner *_wifiScanner;
  BluetoothScanner *_bluetoothScanner;
};

#endif
