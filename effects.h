#ifndef EFFECTS_H
#define EFFECTS_H
#include <iostream>
#include <math.h>


class Effects {
    int16_t* rawData;
    uint32_t size;
    int bitDepth, samplesPerSecond;
    double* convertedSamples;
    public:
        Effects(int16_t* rawData, uint32_t size, int bitDepth, int samplesPerSecond):rawData{rawData},size{size},bitDepth{bitDepth},samplesPerSecond{samplesPerSecond}{};
        Effects() = default;
        void echo(double);
        void normalize();
        void gainAdjustment(double);

        void convertRawData();
};

#endif //EFFECTS_H