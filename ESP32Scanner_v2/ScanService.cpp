#include "ScanService.h"
#include "WiFiScanner.h"
#include "BluetoothScanner.h"
#include "BluetoothDevice.h"
#include "WiFiNetwork.h"
#include <vector>
using namespace std;

#define WIFI 0
#define BT 1

void ScanService::scan(){
  if (_currentScan == WIFI){
    if (_wifiScanner->scanCompleted()){
      //Serial.println("WiFi scan completed");
      autosend(_wifiScanner->getResults());

      _currentScan = BT;
      _bluetoothScanner->scanAsync();
    }
  }
  else if (_currentScan == BT){
    if (_bluetoothScanner->scanCompleted()){
      //Serial.println("Bluetooth scan completed");
      autosend(_bluetoothScanner->getResults());
      
      _currentScan = WIFI;
      _wifiScanner->scanAsync();
    }
  }
}

void ScanService::autosend(vector<WiFiNetwork> networks){
  //Serial.println("Autosending " + String(networks.size()) + " networks");
}
void ScanService::autosend(vector<BluetoothDevice> devices){
  //Serial.println("Autosending " + String(devices.size()) + " devices");
}