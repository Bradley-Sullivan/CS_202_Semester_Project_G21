#include "file.h"

WAV_HEADER WavFile::getWavHeader() const { return wavHeader; }

double* WavFile::getSampleData() const { return sampleData; }

double WavFile::getAudioDuration() const { return (double) (wavHeader.dataBodySize / ((wavHeader.samplesPerSecond * wavHeader.bitsPerSample * wavHeader.numChannels) / 8)); }

uint32_t WavFile::getDataChunkPos() const { return dataChunkPos; }

int WavFile::loadWavHeader(std::string filename) {
    uint8_t chunkID[4];
    uint32_t chunkSize;

    std::cout << "\nLoading file data...\n";

    //creates an ifstream object for reading in binary data from 'filename'
    std::ifstream wavFile (filename, std::ios::binary | std::ios::in);

    //checks that the file is opened correctly
    if (wavFile.is_open()) {
        //Reads in the first 36 bytes of the wav file header
        //The first 36 bytes of a wav header include the RIFF header and the 'fmt ' header+chunk
        wavFile.read((char*)&wavHeader, sizeof(wavHeader) - 8);

        //Simple file format checks. Could possibly adapt current implementation to utilize exceptions and more "correct" error
        //handling methods.
        if (wavHeader.riffID[0] != 'R' && wavHeader.riffID[1] != 'I' && wavHeader.riffID[2] != 'F' && wavHeader.riffID[3] != 'F') {
            std::cout << "\nFile incorrectly formatted. Program requires RIFF .wav files.\n" << std::endl;
            return -1;
        }
        if (wavHeader.formatCode[0] != 'W' && wavHeader.formatCode[1] != 'A' && wavHeader.formatCode[2] != 'V' && wavHeader.formatCode[3] != 'E') {
            std::cout << "\nFile incorrectly formatted. Program requires .wav files with fmt code WAVE.\n" << std::endl;
            return -1;
        }
        if (wavHeader.fmtCode != 1) {
            std::cout << "\nFile incorrectly formatted. Program requires PCM encoded .wav files (i.e. fmt code = 1).\n" << std::endl;
            return -1;
        }
        if (wavHeader.numChannels > 2) {
            std::cout << "\nFile contains more than 2 audio channels. Program will only accept Mono or Stereo .wav files.\n" << std::endl;
            return -1;
        }
        if (wavHeader.bitsPerSample > 16) {
            std::cout << "\nFile contains a bit-depth greater than 16-bit. Program requires 8 to 16 bit .wav files.\n" << std::endl;
            return -1;
        }
        /* 
            This loop reads in the headers of other chunks that might immediately follow the 'fmt '
            chunk. This could include other metadata that relates to the audio file (artist, year, genre, etc.).
            All we're interested in is the 'data' chunk. This loop will read in each chunk header ID and chunk
            size into a buffer, if the chunkID buffer doesn't match the 'data' ID, it will adjust the file pointer
            forward the size of the chunk which is given in the chunk header until it reads in a 'data' chunk ID.
        */
        do {
            wavFile.read((char*)chunkID, sizeof(chunkID));
            wavFile.read((char*)&chunkSize, sizeof(chunkSize));
            if (chunkID[0] != 'd' && chunkID[1] != 'a' && chunkID[2] != 't' && chunkID[3] != 'a') wavFile.seekg(chunkSize + wavFile.tellg());
        } while (chunkID[0] != 'd' && chunkID[1] != 'a' && chunkID[2] != 't' && chunkID[3] != 'a');

        //Copies the contents of the chunkID buffer and chunkSize into the wavHeader struct
        for (int i = 0; i < 4; i++) wavHeader.dataID[i] = chunkID[i];
        wavHeader.dataBodySize = chunkSize;

        dataChunkPos = wavFile.tellg();

        wavFile.close();
    }
    else {
        std::cout << "Unable to open " << filename << " . File does not exist or is unreadable\n" << std::endl;
        return -1;
    }
    return 0;
}

void WavFile::loadSampleData(std::string filename) {
    std::fstream data (filename, std::ios::binary | std::ios::in);
    data.seekg(dataChunkPos);
    if (wavHeader.bitsPerSample == 8) {
        uint8_t* buffer;
        buffer = new uint8_t[wavHeader.dataBodySize];
        data.read((char*)buffer, wavHeader.dataBodySize);
        data.close();
        sampleData = new double[wavHeader.dataBodySize];
/*
    8-bit wav files are always unsigned which means that every value will lies in the range [0,255], NOT
    in [-127,127]. Because we want to center the float values from [-1, 1], we need to subtract 127 from
    every value fixing the range from [0,255] -> [-127,127] and then dividing each value by 127 we then
    transform the range to [-1,1] which is what we want.
*/
        for (uint32_t i = 0; i < wavHeader.dataBodySize; i++) sampleData[i] = (double) (buffer[i] - (INT8_MAX)) / (INT8_MAX);
    }
    else if (wavHeader.bitsPerSample == 16) {
        int16_t* buffer;
        buffer = new int16_t[wavHeader.dataBodySize];
        data.read((char*)buffer, wavHeader.dataBodySize);
        data.close();
        sampleData = new double[wavHeader.dataBodySize];
        for (uint32_t i = 0; i < wavHeader.dataBodySize; i++) sampleData[i] = (double) buffer[i] / INT16_MAX;
    }    
}

void WavFile::writeSampleData(std::string outputFilename) {
    std::ofstream out (outputFilename, std::ofstream::binary);
    if (out.is_open()) {
        if (wavHeader.bitsPerSample == 8) {
            uint8_t* buffer;
            buffer = new uint8_t[wavHeader.dataBodySize];
/*
    Since our values are in the range of [-1,1] and we need them in the range of [0,255] we first
    make sure that after effects are applied, our sample data is clamped between -1 and 1. Then, we
    would like the range to be from [0,1] so that when we scale each sample value by 255 we are
    left with the sample range of [0,255] which is exactly how we read in the data. So we add one
    to every sample so that we go from a range of [-1,1] -> [0,2] and then we divide by two taking
    it to [0,1] and scale by 255 and voila. I spent wayyyyyy too long figuring this out and it's
    not really that complex. :{
*/
            for (uint32_t i = 0; i < wavHeader.dataBodySize; i++) {
                sampleData[i] = clamp(sampleData[i], -1, 1);
                sampleData[i] = (sampleData[i] + 1) / 2;
                sampleData[i] *= 255;
                buffer[i] = sampleData[i];
            }
            out.write((char*)&wavHeader, sizeof(wavHeader));
            out.write((char*)buffer, wavHeader.dataBodySize);
            out.close();
            std::cout << "\nFile saved successfully\n";
        }
        else if (wavHeader.bitsPerSample == 16) {
            int16_t* buffer;
            buffer = new int16_t[wavHeader.dataBodySize];
            for (uint32_t i = 0; i < wavHeader.dataBodySize; i++) buffer[i] = (int16_t) (sampleData[i] * INT16_MAX);
            out.write((char*)&wavHeader, sizeof(wavHeader));
            out.write((char*)buffer, wavHeader.dataBodySize);
            out.close();
            std::cout << "\nFile saved successfully\n";
        }
    }
    else {
        std::cout << "\nError opening output file to save sample data. Could not write changes.\n" << std::endl;
    }
}

double WavFile::clamp(double value, double min, double max) {
    value = std::min(value, max);
    value = std::max(value, min);
    return value;
}