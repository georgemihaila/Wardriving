#ifndef BluetoothScanner_h_
#define BluetoothScanner_h_
#include <vector>
using namespace std;
#include "BluetoothDevice.h"
#include "ScannerBase.h"
#include "ScanChunk.h"
#include "Chunk.h"
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

class BluetoothScanner : public ScannerBase<BluetoothDevice>
{
public:
  BluetoothScanner()
  {
    chunk = new ScanChunk("Bluetooth", 135, 240 / 2 - 20, 0, 120 + 20);

    BLEDevice::init("ESP32 Scanner");
    pBLEScan = BLEDevice::getScan(); //create new scan
    pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
    pBLEScan->setInterval(100);
    pBLEScan->setWindow(99);  // less or equal setInterval value
  };
  virtual void scanAsync();
  virtual bool scanCompleted();
  virtual vector<BluetoothDevice> getResults();
private:
  BLEScan* pBLEScan;

};

#endif
