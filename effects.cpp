#include "effects.h"

void Effects::echo() {
    double delay = 0.5;
    uint32_t offset = delay * samplesPerSecond;
    int16_t buffer[size];
    for (uint32_t i = 0; i < size; i++) {
        buffer[i] = rawData[i];
        if (i > offset) {
            rawData[i] += (int16_t) (buffer[i - offset] * 0.5);
        }
    }
}

void Effects::normalize() {
    double max, normalizationFactor;
    gainAdjustment(0.5);
    convertRawData();
    for (uint32_t i = 0; i < size; i++) {
        if (convertedSamples[i] > max) max = convertedSamples[i];
    }
    normalizationFactor = 1 / max;
    for (uint32_t i = 0; i < size; i++) {
        rawData[i] += (int16_t) (rawData[i] * normalizationFactor);
    }
    delete convertedSamples;
}

void Effects::gainAdjustment(double gainFactor) {
    if (gainFactor < 1) {
        gainFactor = 1 - gainFactor;
        for (uint32_t i = 0; i < size; i++) {
            rawData[i] -= (int16_t) (rawData[i] * gainFactor);
        }
    }
    else {
        for (uint32_t i = 0; i < size; i++) {
            rawData[i] += (int16_t) (rawData[i] * gainFactor);
        }
    }    
}

void Effects::convertRawData() {
    convertedSamples = new double[size];
    for (uint32_t i = 0; i < size; i++) {
        convertedSamples[i] = rawData[i];
        if (bitDepth == 8) convertedSamples[i] /= 255;
        else if (bitDepth == 16) convertedSamples[i] /= 65535;
    }  
}