#ifndef StringableObject_h_
#define StringableObject_h_

#include "Arduino.h"

class StringableObject {
    public:
        virtual String toString();
};

#endif
