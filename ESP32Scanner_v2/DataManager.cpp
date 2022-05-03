#include "DataManager.h"
#include "ArduinoJson.h"
#define CACHE_SIZE 50

#define WIFI_FILENAME "/WiFi.csv"
#define BT_FILENAME "/Bluetooth.csv"

bool DataManager::updateChunk()
{
    int x = int(_gpsService->getXMetersFromOrigin() / _chunkLengthMeters);
    int y = int(_gpsService->getYMetersFromOrigin() / _chunkLengthMeters);

    bool updated = false;
    if (x != _currentXChunk)
    {
        updated = true;
        _currentXChunk = x;
    }
    if (y != _currentYChunk)
    {
        updated = true;
        _currentYChunk = y;
    }
    if (updated)
    {
        Serial.println("Position changed, current chunk - X: " + String(x) + " Y: " + String(y));
    }
    return updated;
}

String DataManager::getChunkName()
{
    return String(_currentXChunk) + "x" + String(_currentYChunk);
}

String DataManager::getChunkFileName(String dataType)
{
    return "/" + dataType + " " + String(_chunkLengthMeters) + "x" + String(_chunkLengthMeters) + " " + getChunkName() + ".csv";
}

int DataManager::saveNewEntries(vector<WiFiNetwork> networks)
{
    updateChunk();
    return cacheNewEntriesToSD(networks);
}

int DataManager::saveNewEntries(vector<BluetoothDevice> devices)
{
    updateChunk();
    return cacheNewEntriesToSD(devices);
}

bool DataManager::listHasElement(String list[CACHE_SIZE], int size, String s)
{
    for (int i = 0; i < size; i++)
    {
        if (list[i].equals(s))
        {
            return true;
        }
    }
    return false;
}

void DataManager::addToCache(String (&cache)[CACHE_SIZE], int &size, String s)
{
    // delete(cache[size % CACHE_SIZE]);
    incrementCurrentlyCachedIfNotFull(size);
    cache[size % CACHE_SIZE] = s;
}

void DataManager::incrementCurrentlyCachedIfNotFull(int &count)
{
    if (count < CACHE_SIZE - 1)
    {
        count++;
    }
}

int DataManager::cacheNewEntriesToSD(vector<WiFiNetwork> networks)
{
    int added = 0;
    for (int i = 0; i < networks.size(); i++)
    {
        if (!listHasElement(_wifiChunkCache, _currentlyCachedWifi, networks[i].BSSID))
        {
            String s = networks[i].toString() + ',' + _gpsService->generateLocationCSV();
            _sdCard->appendFile(getChunkFileName(WIFI_FILENAME), s);
            //_sdCard->appendFile(WIFI_FILENAME, s);
            addToCache(_wifiChunkCache, _currentlyCachedWifi, networks[i].BSSID);
            added++;
        }
    }
    return added;
}

int DataManager::cacheNewEntriesToSD(vector<BluetoothDevice> devices)
{
    int added = 0;
    for (int i = 0; i < devices.size(); i++)
    {
        if (!listHasElement(_btChunkCache, _currentlyCachedBluetooth, devices[i].address))
        {
            String s = _gpsService->generateLocationCSV() + "," + devices[i].toString();
            _sdCard->appendFile(getChunkFileName(BT_FILENAME), s);
            //_sdCard->appendFile(BT_FILENAME, s);
            addToCache(_btChunkCache, _currentlyCachedBluetooth, devices[i].address);
            added++;
        }
    }
    return added;
}

void lineRead(String line)
{
}

void DataManager::sendCollectedDataToServer(TFTDisplay *display)
{
    display->printAt("WiFi data...", 0, 80);
    _sdCard->sendAllLinesFor("WiFi", _api, display, 0);
    display->printAt("Bluetooth data...", 0, 120);
    _sdCard->sendAllLinesFor("Bluetooth", _api, display, 0);
}

Config DataManager::getConfig()
{
    String input = _sdCard->readFile("/config.json");
    StaticJsonDocument<96> doc;

    input.trim();
    Serial.println("JSON config file: |" + input + "|");
    DeserializationError error = deserializeJson(doc, input);
    Config result;
    if (error)
    {
        Serial.print("deserializeJson() failed: ");
        Serial.println(error.c_str());
        setNumberOfTotalDevicesFound(5217, 5258);
        result.totalWiFiNetworks = 5217;
        result.totalBluetoothDevices = 5258;
    }
    else
    {
        int totalWiFiNetworks = doc["totalWiFiNetworks"];         // 0
        int totalBluetoothDevices = doc["totalBluetoothDevices"]; // 0
        result.totalWiFiNetworks = totalWiFiNetworks;
        result.totalBluetoothDevices = totalBluetoothDevices;
    }
    return result;
}

void DataManager::setNumberOfTotalDevicesFound(int wifi, int bt)
{
    StaticJsonDocument<32> doc;
    doc["totalWiFiNetworks"] = wifi;
    doc["totalBluetoothDevices"] = bt;
    String output;
    serializeJson(doc, output);
    _sdCard->writeFile("/config.json", output);
}