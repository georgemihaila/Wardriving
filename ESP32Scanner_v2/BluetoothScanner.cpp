#include <vector>
using namespace std;
#include "BluetoothDevice.h"
#include "BluetoothScanner.h"
#include "BLEDevice.h"
#include "BLEAdvertisedDevice.h"
#include <ctype.h>

bool _isScanRunning;
vector<BluetoothDevice> _btDevices;

String filterOutNonASCIICharacters(const char *s)
{
  String result = "";
  for (int i = 0; i < strlen(s); i++)
  {
    if (isAscii(s[i]))
    {
      result += s[i];
    }
  }

  return result;
}

class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks
{
  void onResult(BLEAdvertisedDevice advertisedDevice)
  {
    BluetoothDevice device;
    device.name = String(advertisedDevice.getName().c_str());
    device.address = String(advertisedDevice.getAddress().toString().c_str());
    device.manufacturerData = String(advertisedDevice.getManufacturerData().c_str());
    //device.serviceUUID = filterOutNonASCIICharacters(advertisedDevice.getServiceUUID().toString().c_str());
    _btDevices.push_back(device);
  }
};

static void callback(BLEScanResults scanResults)
{
  _isScanRunning = false;
  scanResults.dump();
}

void BluetoothScanner::scanAsync()
{
  _isScanRunning = true;
  _btDevices.clear();
  _lastScanStartedAt = millis();

  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true);
  pBLEScan->start(1, callback);
}

vector<BluetoothDevice> BluetoothScanner::getResults()
{
  return _btDevices;
}

bool BluetoothScanner::scanCompleted()
{
  return !_isScanRunning;
}