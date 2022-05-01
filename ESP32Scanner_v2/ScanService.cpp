#include "ScanService.h"
#include "WiFiScanner.h"
#include "BluetoothScanner.h"
#include "BluetoothDevice.h"
#include "WiFiNetwork.h"
#include <vector>
using namespace std;

#define WIFI 0
#define BT 1

void ScanService::scan()
{
  if (_currentScan == WIFI)
  {
    if (_wifiScanner->scanCompleted())
    {
      _wifiScanner->chunk->totalScans++;

      int newWiFis = _dataManager->autosend(_wifiScanner->getResults());
      _wifiScanner->chunk->addNSatellites(_gpsService->nSatellites);
      _wifiScanner->chunk->newNetworks = newWiFis;
      _wifiScanner->chunk->totalNetworks += newWiFis;

      _currentScan = BT;
      //Serial.println("Starting Bluetooth scan");
      _bluetoothScanner->scanAsync();
    }
  }
  else if (_currentScan == BT)
  {
    if (_bluetoothScanner->scanCompleted())
    {
      _bluetoothScanner->chunk->totalScans++;
      _bluetoothScanner->chunk->networksAround = _bluetoothScanner->getResults().size();

      int newBTs = _dataManager->autosend(_bluetoothScanner->getResults());
      _bluetoothScanner->chunk->addNSatellites(_gpsService->nSatellites); //Maybe we could not use two arrays
      _bluetoothScanner->chunk->newNetworks = newBTs;
      _bluetoothScanner->chunk->totalNetworks += newBTs;

      _currentScan = WIFI;
      //Serial.println("Starting WiFi scan");
      _wifiScanner->scanAsync();
    }
  }
}