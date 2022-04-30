#include <vector>
using namespace std;
#include "BluetoothDevice.h"
#include "BluetoothScanner.h"

void BluetoothScanner::scanAsync(){
  _isScanRunning = true;
}

vector<BluetoothDevice> BluetoothScanner::getResults() {
  return vector<BluetoothDevice>();
}

bool BluetoothScanner::scanCompleted(){
  if (_isScanRunning){
    _isScanRunning = false;
  }
  return true;
}