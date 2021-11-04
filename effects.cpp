#include "effects.h"

void Effects::echo(double audioDuration) {
    double echoDelaySeconds, echoDecay = 0.5;
    do {
        std::cout << "\nPlease enter echo delay in seconds.\n> ";
        std::cin >> echoDelaySeconds;
        if (echoDelaySeconds >= audioDuration) {
            std::cout << "Invalid delay. Echo delay must be less than the audio file's duration\n\tAudio Duration: " << audioDuration << " seconds" << std::endl;
        }
    } while(echoDelaySeconds >= audioDuration);
    int delayOffset = (int) (echoDelaySeconds * samplesPerSecond);
    for (uint32_t i = 0; i < size - delayOffset; i++) {
        if (i + delayOffset < size) rawData[i + delayOffset] += (int16_t) (rawData[i] * echoDecay);
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