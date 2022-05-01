#ifndef API_h_
#define API_h_

#include "Arduino.h"
#include <WiFi.h>
#include <HTTPClient.h>

class API
{
    public:
        API(String endpoint){
            _endpoint = endpoint;
        }
        int createNewSession();
    private:
        String _endpoint;
        int postJSON(String path, String json);
        HTTPClient _httpClient;
        WiFiClient _wifiClient;
};

#endif
