#ifndef UI_H
#define UI_H

#include <iostream>
#include <string>
#include "file.h"
#include "effects.h"

class UI {
    WAV_HEADER header;
    public:
        int startMenu();
        std::string requestFilename();
        void printWavMetadata(std::string, WAV_HEADER);
        int processorMenu();
        std::string requestOutputFilename();
        void requestGainFactor();
        void requestEchoDelay();
};

#endif //UI_H