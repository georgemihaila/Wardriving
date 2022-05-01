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

      autosend(_wifiScanner->getResults());
      _wifiScanner->chunk->addNSatellites(_gpsService->nSatellites);

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

      autosend(_bluetoothScanner->getResults());
      _bluetoothScanner->chunk->addNSatellites(_gpsService->nSatellites); //Maybe we could not use two arrays

      _currentScan = WIFI;
      //Serial.println("Starting WiFi scan");
      _wifiScanner->scanAsync();
    }
  }
}

void ScanService::autosend(vector<WiFiNetwork> networks)
{
  //_wifiScanner->chunk->totalNetworks += totalNew(networks.size()); // Remember to filter this (add only new networks)
  // Serial.println("Autosending " + String(networks.size()) + " networks");
}
void ScanService::autosend(vector<BluetoothDevice> devices)
{
  // Serial.println("Autosending " + String(devices.size()) + " devices");
}