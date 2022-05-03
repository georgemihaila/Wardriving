#ifndef ThreeWaySwitch_h_
#define ThreeWaySwitch_h_

#include <Arduino.h>

class ThreeWaySwitch
{
public:
    ThreeWaySwitch(int pin1, int pin2, int defaultState)
    {
        _pin1 = pin1;
        _pin2 = pin2;
        _defaultState = defaultState;

        pinMode(pin1, INPUT);
        pinMode(pin2, INPUT);
    }

    int getState()
    {
        int p1 = digitalRead(_pin1);
        int p2 = digitalRead(_pin2);

        if (p1 == _defaultState && p2 == _defaultState)
        {
            return 1; //middle
        }
        if (p1 == _defaultState)
        {
            return 2; //side
        }
        return 0; //other side
    }

private:
    int _pin1;
    int _pin2;
    int _defaultState;
};

#endif
