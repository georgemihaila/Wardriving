#ifndef DataManager_h_
#define DataManager_h_

#include "SDCard.h"
#include "GPSService.h"
#include "WiFiNetwork.h"
#include "BluetoothDevice.h"
#include "API.h"

class DataManager
{
public:
    DataManager(SDCard *sdCard, GPSService *gpsService, API *api)
    {
        _sdCard = sdCard;
        _gpsService = gpsService;
        _api = api;
    }
    int autosend(vector<WiFiNetwork> networks);
    int autosend(vector<BluetoothDevice> devices);

private:
    SDCard *_sdCard;
    GPSService *_gpsService;
    API *_api;

    int _chunkLengthMeters = 30; // Square chunks
    bool isOriginChunk(){
        return _currentXChunk == 0 && _currentYChunk == 0;
    }
    int _currentXChunk = -1;
    int _currentYChunk = -1;
    bool updateChunk();
    vector<String> _wifiChunkCache;
    vector<String> _btChunkCache;
    void reloadCache();
    void updateChunkAndReloadCache();
    String getChunkName();
    String getChunkFileName(String dataType);
    bool listHasElementContaining(vector<String> list, String s);
    int cacheNewEntriesToSD(vector<WiFiNetwork> networks);
    int cacheNewEntriesToSD(vector<BluetoothDevice> devices);
};

#endif
