#include "effects.h"

void Effects::normalize(double* rawData, WAV_HEADER wavHeader) {
    double max = 0, normFactor;
    uint32_t maxLocation;
    // rawData[x] is signed and need to find the index of rawData with the greatest absolute value
    for (uint32_t i = 0; i < wavHeader.dataBodySize; i++) {
        if (abs(rawData[i]) > max) {
            max = abs(rawData[i]);
            maxLocation = i;
        }
    }
    std::cout << max << "x\n\n";
    normFactor = 1 / rawData[maxLocation];
    for (uint32_t i = 0; i < wavHeader.dataBodySize; i++) rawData[i] *= normFactor;
}

void Effects::echo(double* rawData, WAV_HEADER wavHeader) {
    double echoDelaySeconds, echoDecay = 0.4;
    double audioDuration = (wavHeader.dataBodySize / ((wavHeader.samplesPerSecond * wavHeader.bitsPerSample * wavHeader.numChannels) / 8));
    do {
        std::cout << "\nPlease enter echo delay in seconds.\n> ";
        std::cin >> echoDelaySeconds;
        if (echoDelaySeconds >= audioDuration) {
            std::cout << "Invalid delay. Echo delay must be less than the audio file's duration\n\tAudio Duration: " << audioDuration << " seconds" << std::endl;
        }
    } while(echoDelaySeconds >= audioDuration);
    int delayOffset = echoDelaySeconds * wavHeader.samplesPerSecond;
    for (uint32_t i = 0; i < wavHeader.dataBodySize - delayOffset; i++) {
        if (i + delayOffset < wavHeader.dataBodySize) {
            rawData[i + delayOffset] += rawData[i] * echoDecay;
        }
    }
}

void Effects::gainAdjustment(double* rawData, WAV_HEADER wavHeader) {
    double gainFactor;
    do {
        std::cout << "\nPlease enter your desired gain adjustment factor (i.e. a value betwen 0 & 1)" << std::endl;
        std::cout << "> ";
        std::cin >> gainFactor;
        if (gainFactor < 0 || gainFactor > 1) std::cout << "\nInvalid value entered. Try again" << std::endl;
    } while(gainFactor < 0 || gainFactor > 1);
    for (uint32_t i = 0; i < wavHeader.dataBodySize; i++) rawData[i] *= gainFactor;
}
