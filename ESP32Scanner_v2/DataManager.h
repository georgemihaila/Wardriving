#ifndef DataManager_h_
#define DataManager_h_

#include "SDCard.h"
#include "GPSService.h"
#include "WiFiNetwork.h"
#include "BluetoothDevice.h"
#include "API.h"
#include "TFTDisplay.h"
#include "Config.h"

#define CACHE_SIZE 50
class DataManager
{
public:
    DataManager(SDCard *sdCard, GPSService *gpsService, API *api)
    {
        _sdCard = sdCard;
        _gpsService = gpsService;
        _api = api;

        _sdCard->createDir("/WiFi/");
        _sdCard->createDir("/Bluetooth/");
    }
    int saveNewEntries(vector<WiFiNetwork> networks, bool noLog);
    int saveNewEntries(vector<BluetoothDevice> devices, bool noLog);
    void sendCollectedDataToServer(TFTDisplay *display);
    void listDir(fs::FS &fs, const char * dirname, uint8_t levels);
    Config getConfig();
    void setNumberOfTotalDevicesFound(int wifi, int bt);
private:
    SDCard *_sdCard;
    GPSService *_gpsService;
    API *_api;

    int _chunkLengthMeters = 100; // Square chunks
    bool isOriginChunk()
    {
        return _currentXChunk == 0 && _currentYChunk == 0;
    }
    int _currentXChunk = -1;
    int _currentYChunk = -1;
    bool updateChunk();
    int _currentlyCachedWifi = 0;
    int _currentlyCachedBluetooth = 0;
    String _wifiChunkCache[CACHE_SIZE];
    String _btChunkCache[CACHE_SIZE];
    String getChunkName();
    String getChunkFileName(String dataType);
    void incrementCurrentlyCachedIfNotFull(int &count);
    bool listHasElement(String list[CACHE_SIZE], int size, String s);
    void addToCache(String (&cache)[CACHE_SIZE], int &size, String s);
    int cacheNewEntriesToSD(vector<WiFiNetwork> networks, bool noLog);
    int cacheNewEntriesToSD(vector<BluetoothDevice> devices, bool noLog);
};

#endif
