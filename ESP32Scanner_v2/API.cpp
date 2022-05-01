#include "API.h"

int API::postJSON(String path, String json)
{
    String serverPath = _endpoint + path;
    _httpClient.begin(_wifiClient, serverPath);
    _httpClient.addHeader("Content-Type", "application/json");
    int httpResponseCode = _httpClient.POST(json);
    _httpClient.end();
    Serial.println("POST " + path + " - " + String(httpResponseCode));
    return httpResponseCode;
}

int API::createNewSession()
{
    return postJSON("Session/CreateNewSession", "{}");
}