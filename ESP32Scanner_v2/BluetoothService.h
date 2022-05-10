#ifndef BluetoothService_h_
#define BluetoothService_h_

#include "BluetoothSerial.h"
#include "esp_bt_device.h"

class BluetoothService
{
public:
    BluetoothService()
    {
        SerialBT.begin("ESP32WD"); // Bluetooth device name

        Serial.println("The device started, now you can pair it with bluetooth!");
        Serial.println("Device Name: ESP32test");
        Serial.print("BT MAC: ");
        printDeviceAddress();
        Serial.println();
    };
    void printDeviceAddress()
    {

        const uint8_t *point = esp_bt_dev_get_address();

        for (int i = 0; i < 6; i++)
        {

            char str[3];

            sprintf(str, "%02X", (int)point[i]);
            Serial.print(str);

            if (i < 5)
            {
                Serial.print(":");
            }
        }
    }
    void yield()
    {
        if (Serial.available())
        {
            SerialBT.write(Serial.read());
        }
        if (SerialBT.available())
        {
            Serial.write(SerialBT.read());
        }
    }

private:
    BluetoothSerial SerialBT;
};

#endif
