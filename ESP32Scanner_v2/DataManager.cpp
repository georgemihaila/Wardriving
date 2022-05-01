#include "DataManager.h"

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
    return "/" + dataType + String(_chunkLengthMeters) + "x" + String(_chunkLengthMeters) + " chunk" + getChunkName() + ".csv";
}

void DataManager::reloadCache()
{
    _wifiChunkCache.clear();
    _wifiChunkCache = _sdCard->readAllLines(getChunkFileName("WiFi"));

    _btChunkCache.clear();
    _btChunkCache = _sdCard->readAllLines(getChunkFileName("Bluetooth"));
}

void DataManager::updateChunkAndReloadCache()
{
    if (updateChunk())
    {
        reloadCache();
    }
}

int DataManager::autosend(vector<WiFiNetwork> networks)
{
    updateChunkAndReloadCache();
    // if (!isOriginChunk()) // Don't save data on SD if origin chunk (@[0, 0])
    {
        return cacheNewEntriesToSD(networks);
    }
    return 0;
}

int DataManager::autosend(vector<BluetoothDevice> devices)
{
    updateChunkAndReloadCache();
    // if (!isOriginChunk())
    {
        return cacheNewEntriesToSD(devices);
    }
    return 0;
}

bool DataManager::listHasElementContaining(vector<String> list, String s)
{
    for (int i = 0; i < list.size(); i++)
    {
        if (list[i].indexOf(s) > 0)
        {
            return true;
        }
    }
    return false;
}

int DataManager::cacheNewEntriesToSD(vector<WiFiNetwork> networks)
{
    int added = 0;
    for (int i = 0; i < networks.size(); i++)
    {
        if (!listHasElementContaining(_wifiChunkCache, networks[i].BSSID))
        {
            String s = networks[i].toString() + ',' + _gpsService->generateLocationCSV();
            _sdCard->appendFile(getChunkFileName("WiFi"), s);
            _wifiChunkCache.push_back(s);
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
        if (!listHasElementContaining(_btChunkCache, devices[i].address))
        {
            String s = _gpsService->generateLocationCSV() + "," + devices[i].toString();
            _sdCard->appendFile(getChunkFileName("Bluetooth"), s);
            _btChunkCache.push_back(s);
            added++;
        }
    }
    return added;
}