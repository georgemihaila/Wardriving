#ifndef API_h_
#define API_h_

#include "Arduino.h"

class API
{
    public:
        API(String endpoint){
            _endpoint = endpoint;
        }
    private:
        String _endpoint;
};

#endif
