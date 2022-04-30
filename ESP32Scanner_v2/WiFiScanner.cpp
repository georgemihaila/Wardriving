#include <vector>
using namespace std;
#include "WiFiNetwork.h"
#include "WiFiScanner.h"
#include <WiFi.h>

String bssidToString(uint8_t *bssid){
  char HexLookUp[] = "0123456789abcdef";
  String result;
  for (int i = 0; i < 6; i++){
    if (i % 2 == 1){
      result += ":";
    }
    result += HexLookUp[bssid[i] >> 4];
    result += HexLookUp[bssid[i] & 0x0F];
  }
  return result;
}

vector<WiFiNetwork> WiFiScanner::scan(){
  vector<WiFiNetwork> result;
  int n = WiFi.scanNetworks(false, true);
  wiFiNetworksAround = n;
  for (int i = 0; i < n; ++i) {
    WiFiNetwork network;
    network.SSID = WiFi.SSID(i);
    network.encryptionType = WiFi.encryptionType(i);
    network.channel = WiFi.channel(i);
    network.BSSID = bssidToString(WiFi.BSSID(i));
    result.push_back(network);
  }
  return result;
}