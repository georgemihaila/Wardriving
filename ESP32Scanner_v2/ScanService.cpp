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

      vector<WiFiNetwork> networks = _wifiScanner->getResults();
      int newWiFis = _dataManager->saveNewEntries(networks, _modeThreeWaySwitch->getState() == 2);
      _wifiScanner->chunk->addNSatellites(_gpsService->nSatellites);
      _wifiScanner->chunk->newNetworks = newWiFis;
      _wifiScanner->chunk->totalNetworks += newWiFis;
      _wifiScanner->chunk->lastScanTimeMs = _wifiScanner->getTimeSinceLastScanStarted();

      // Autosend if home after first scan, don't try again otherwise. Should speed up boot time significantly
      if (!firstWifiScanCompleted)
      {
        if (_modeThreeWaySwitch->getState() != 0) // no sending in QS mode
        {
          if (_wifiScanner->homeNetworkAround())
          {
            _autosendFunction();
          }
        }
        firstWifiScanCompleted = true;
      }

      // Don't do BT scans if we're moving relatively fast, we care more about WiFi
      if (_gpsService->speedMetersPerSecond < 5 && _scanTypeThreeWaySwitch->getState() != 2)
      {
        _currentScan = BT;
        _bluetoothScanner->scanAsync();
      }
      else
      {
        _wifiScanner->scanAsync();
      }
    }
  }
  else if (_currentScan == BT)
  {
    if (_bluetoothScanner->scanCompleted())
    {
      _bluetoothScanner->chunk->totalScans++;
      
      vector<BluetoothDevice> devices = _bluetoothScanner->getResults();
      _bluetoothScanner->chunk->networksAround = devices.size();
      int newBTs = _dataManager->saveNewEntries(devices, _modeThreeWaySwitch->getState() == 2);
      _bluetoothScanner->chunk->addNSatellites(_gpsService->nSatellites); // Maybe we could not use two arrays
      _bluetoothScanner->chunk->newNetworks = newBTs;
      _bluetoothScanner->chunk->totalNetworks += newBTs;
      _bluetoothScanner->chunk->lastScanTimeMs = _bluetoothScanner->getTimeSinceLastScanStarted();

      _currentScan = WIFI;
      _wifiScanner->scanAsync();
    }
  }
  if (_modeThreeWaySwitch->getState() != 2){
    _cacheTotalNumberOfDevices();
  }
}

void ScanService::_cacheTotalNumberOfDevices()
{
  if (millis() - _lastTotalNumberOfScansCacheTimestamp >= _cacheTotalNumberOfDevicesEvery)
  {
    if (_lastCachedAtWifi != _wifiScanner->chunk->totalNetworks && _lastCachedAtBT != _bluetoothScanner->chunk->totalNetworks)
    { // Don't write to SD unless new devices found
      _lastCachedAtWifi = _wifiScanner->chunk->totalNetworks;
      _lastCachedAtBT = _bluetoothScanner->chunk->totalNetworks;
      _dataManager->setNumberOfTotalDevicesFound(_lastCachedAtWifi, _lastCachedAtBT);
      // Serial.println("Updated total number of scans");
    }
    _lastTotalNumberOfScansCacheTimestamp = millis();
  }
}
