#ifndef EFFECTS_H
#define EFFECTS_H
#include <iostream>
#include <math.h>
#include "file.h"

class Effects {
    float* rawData;
    WAV_HEADER wavHeader;
    
    public:
        Effects(float* rawData, WAV_HEADER header):rawData{rawData},wavHeader{header}{};
        Effects() = default;
        void echo(double);
        void normalize();
        void gainAdjustment();
};

#endif //EFFECTS_H