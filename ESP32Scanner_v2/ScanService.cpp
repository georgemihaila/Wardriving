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

      int newWiFis = _dataManager->saveNewEntries(_wifiScanner->getResults());
      _wifiScanner->chunk->addNSatellites(_gpsService->nSatellites);
      _wifiScanner->chunk->newNetworks = newWiFis;
      _wifiScanner->chunk->totalNetworks += newWiFis;

      //Autosend if home after first scan, don't try again otherwise. Should speed up boot time significantly
      if (!firstWifiScanCompleted)
      {
        _autosendFunction();
        firstWifiScanCompleted = true;
      }

      _currentScan = BT;
      // Serial.println("Starting Bluetooth scan");
      _bluetoothScanner->scanAsync();
    }
  }
  else if (_currentScan == BT)
  {
    if (_bluetoothScanner->scanCompleted())
    {
      _bluetoothScanner->chunk->totalScans++;
      _bluetoothScanner->chunk->networksAround = _bluetoothScanner->getResults().size();

      int newBTs = _dataManager->saveNewEntries(_bluetoothScanner->getResults());
      _bluetoothScanner->chunk->addNSatellites(_gpsService->nSatellites); // Maybe we could not use two arrays
      _bluetoothScanner->chunk->newNetworks = newBTs;
      _bluetoothScanner->chunk->totalNetworks += newBTs;

      _currentScan = WIFI;
      // Serial.println("Starting WiFi scan");
      _wifiScanner->scanAsync();
    }
  }
  _cacheTotalNumberOfScans();
}

void ScanService::_cacheTotalNumberOfScans()
{
  if (millis() - _lastTotalNumberOfScansCacheTimestamp >= _cacheTotalNumberOfScansEvery)
  {
    if (_lastCachedAtWifi != _wifiScanner->chunk->totalNetworks && _lastCachedAtBT != _bluetoothScanner->chunk->totalNetworks)
    { //Don't write to SD unless new devices found
      _lastCachedAtWifi = _wifiScanner->chunk->totalNetworks;
      _lastCachedAtBT = _bluetoothScanner->chunk->totalNetworks;
      _dataManager->setNumberOfTotalDevicesFound(_lastCachedAtWifi, _lastCachedAtBT);
      Serial.println("Updated total number of scans");
    }
    _lastTotalNumberOfScansCacheTimestamp = millis();
  }
}