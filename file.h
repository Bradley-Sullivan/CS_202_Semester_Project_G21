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
    double* sampleData;

    public:
        /**
         * @brief Get the Wav Header struct
         * 
         * @return WAV_HEADER 
         */
        WAV_HEADER getWavHeader() const;

        /**
         * @brief Get the Sample Data array
         * 
         * @return double* 
         */
        double* getSampleData() const;  

        /**
         * @brief Get the Audio Duration
         * 
         * @return double 
         */
        double getAudioDuration() const;

        /**
         * @brief Get the Data Chunk Position in the file
         * 
         * @return uint32_t 
         */
        uint32_t getDataChunkPos() const;

        /**
         * @brief Loads the header/chunk info from the wav file
         * 
         * @return int 
         */
        int loadWavHeader(std::string);

        /**
         * @brief Picks up where loadWavHeader() left off and reads in sample data into an array
         * 
         */
        void loadSampleData(std::string);

        /**
         * @brief Saves modified sample data to an output file
         * 
         */
        void writeSampleData(std::string);

        /**
         * @brief Clamps a value between two bounds
         * 
         * @return double 
         */
        double clamp(double, double, double);
};

#endif //FILE_H