/**
 * @file file.h
 * @authors Bradley Sullivan, Emily Cuellar, Ever McNeill
 * @brief Header file for the WavFile class which handles the loading and saving of .wav file sample data.
 * @date 2021-11-10
 */
#ifndef FILE_H
#define FILE_H

#include <iostream>
#include <fstream>
#include <string>
#include <math.h>

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
    uint32_t dataChunkPos;
    float* sampleData;

    public:        
        WAV_HEADER getWavHeader() const;
        float* getSampleData() const;   
        double getAudioDuration() const;
        uint32_t getDataChunkPos() const;
        int loadWavHeader(std::string);
        void loadSampleData(std::string);
        void writeSampleData(std::string);
};

#endif //FILE_H