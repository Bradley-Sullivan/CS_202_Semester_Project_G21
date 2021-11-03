#ifndef UI_H
#define UI_H

#include <iostream>
#include <string>
#include "file.h"
#include "effects.h"

class UI {
    WavFile audioFile;
    WAV_HEADER header;
    std::string filename;
    std::string outputFilename;
    public:
        int startMenu();
        void requestFilename();
        void printWavMetadata();
        void processorMenu();
        void requestOutputFilename();
        void requestGainFactor();
        void requestEchoDelay();
};

#endif //UI_H