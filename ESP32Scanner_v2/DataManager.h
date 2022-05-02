#ifndef DataManager_h_
#define DataManager_h_

#include "SDCard.h"
#include "GPSService.h"
#include "WiFiNetwork.h"
#include "BluetoothDevice.h"
#include "API.h"

#define CACHE_SIZE 50
class DataManager
{
public:
    DataManager(SDCard *sdCard, GPSService *gpsService, API *api)
    {
        _sdCard = sdCard;
        _gpsService = gpsService;
        _api = api;
    }
    int saveNewEntries(vector<WiFiNetwork> networks);
    int saveNewEntries(vector<BluetoothDevice> devices);

private:
    SDCard *_sdCard;
    GPSService *_gpsService;
    API *_api;

    int _chunkLengthMeters = 10; // Square chunks; 30x30 seems to be too large
    bool isOriginChunk(){
        return _currentXChunk == 0 && _currentYChunk == 0;
    }
    int _currentXChunk = -1;
    int _currentYChunk = -1;
    bool updateChunk();
    int _currentlyCachedWifi= 0;
    int _currentlyCachedBluetooth = 0;
    String _wifiChunkCache[CACHE_SIZE];
    String _btChunkCache[CACHE_SIZE];
    void reloadCache();
    void updateChunkAndReloadCache();
    String getChunkName();
    String getChunkFileName(String dataType);
    void incrementCurrentlyCachedIfNotFull(int& count);
    bool listHasElement(String list[CACHE_SIZE], int size, String s);
    void addToCache(String (&cache)[CACHE_SIZE], int &size, String s);
    int cacheNewEntriesToSD(vector<WiFiNetwork> networks);
    int cacheNewEntriesToSD(vector<BluetoothDevice> devices);
};

#endif
