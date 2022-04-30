#include "ScanService.h"
#include "WiFiScanner.h"
#include "BluetoothScanner.h"
#include "BluetoothDevice.h"
#include "WiFiNetwork.h"
#include <vector>
using namespace std;

void ScanService::scan(){
  autosend(_wifiScanner->scan());
  autosend(_bluetoothScanner->scan());
}

void ScanService::autosend(vector<WiFiNetwork> networks){

}
void ScanService::autosend(vector<BluetoothDevice> devices){

}