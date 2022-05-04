#include <vector>
using namespace std;
#include "BluetoothDevice.h"
#include "BluetoothScanner.h"
#include "BLEDevice.h"
#include "BLEAdvertisedDevice.h"

bool _isScanRunning;
vector<BluetoothDevice> _btDevices;

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
	void onResult(BLEAdvertisedDevice advertisedDevice) {
    BluetoothDevice device;
    device.name = String(advertisedDevice.getName().c_str());
    device.address = String(advertisedDevice.getAddress().toString().c_str());
    device.manufacturerData = String(advertisedDevice.getManufacturerData().c_str());
    device.serviceUUID = String(advertisedDevice.getServiceUUID().toString().c_str());
    _btDevices.push_back(device);
	}
};

static void callback(BLEScanResults scanResults) {
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