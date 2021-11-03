#ifndef FILE_H
#define FILE_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#define MAX_NORM_16 65535
#define MAX_NORM_8 255

typedef struct WAV_HEADER{
    //RIFF Chunk
    uint8_t riffID[4];
    uint32_t riffBodySize;
    uint8_t formatCode[4];
    //fmt Chunk
    uint8_t fmtID[4];
    uint32_t fmtBodySize;
    uint16_t fmtCode;
    uint16_t numChannels;
    uint32_t samplesPerSecond;
    uint32_t bytesPerSecond;
    uint16_t bytesPerFrame;
    uint16_t bitsPerSample;
    //data Chunk
    uint8_t dataID[4];
    uint32_t dataBodySize;
} WAV_HEADER;


class WavFile {
    WAV_HEADER wavHeader;
    int16_t* sampleData;

    public:
        int16_t* getSampleData() const;
        WAV_HEADER getWavHeader() const;

        int loadWavData(std::string filename);
        void saveFile(std::string);
};

#endif //FILE_H