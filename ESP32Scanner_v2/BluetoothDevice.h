#ifndef BluetoothDevice_h_
#define BluetoothDevice_h_

#include "Arduino.h"

class BluetoothDevice
{
    public:
        String name;
        String address;
        String manufacturerData;
        String serviceUUID;
};

#endif
