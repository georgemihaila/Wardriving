#include "API.h"

int API::postJSON(String path, String json)
{
    String serverPath = _endpoint + path;
    _httpClient.begin(_wifiClient, serverPath);
    _httpClient.addHeader("Content-Type", "application/json");
    int httpResponseCode = _httpClient.POST(json);
    _httpClient.end();
    Serial.println("POST |" + json + "|" + serverPath + " - " + String(httpResponseCode));
    if (httpResponseCode < 0)
    {
        Serial.println(_httpClient.errorToString(httpResponseCode).c_str());
    }
    return httpResponseCode;
}

int API::createNewSession()
{
    return postJSON("Session/CreateNewSession", "{}");
}

int API::postData(String data, String dataType)
{
    bool isJSON = data[0] == '"';
    if (!isJSON)
    {
        data = "\"" + data + "\"";
    }
    return postJSON(dataType + "/ProcessRawString", data);
}