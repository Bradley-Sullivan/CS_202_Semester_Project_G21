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
#include <vector>

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
        /**
         * @brief Returns a copy of the sampleData pointer
         * 
         * @return int16_t*
         */
        int16_t* getSampleData() const;
        /**
         * @brief Returns a copy of the WAV_HEADER struct
         * 
         * @return WAV_HEADER 
         */
        WAV_HEADER getWavHeader() const;
        /**
         * @brief Calculates and returns the duration of the audio file
         * 
         * @return double 
         */
        double getAudioDuration() const;

        /**
         * @brief Loads the file data into an int16_t buffer
         * 
         * @param filename - Name of the .wav file
         * @return int - Returns 0 if the file is correctly formatted and loaded. Returns -1 if not.
         */
        int loadWavData(std::string filename);
        /**
         * @brief Saves modified sample data into an existing file. If an existing file doesn't exist, it creates one.
         * 
         * @param outputFilename - Name of the file to be written to disk
         */
        void saveFile(std::string outputFilename);
};

#endif //FILE_H