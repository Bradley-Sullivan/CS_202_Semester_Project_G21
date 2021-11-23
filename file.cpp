#include "file.h"

WAV_HEADER WavFile::getWavHeader() const { return wavHeader; }

float* WavFile::getSampleData() const { return sampleData; }

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
    std::ifstream data (filename, std::ios::binary | std::ios::in);
    data.seekg(dataChunkPos);
    if (wavHeader.bitsPerSample == 8) {
        int8_t* buffer;
        buffer = new int8_t[wavHeader.dataBodySize];
        data.read((char*)buffer, wavHeader.dataBodySize);
        data.close();
        sampleData = new float[wavHeader.dataBodySize];
        for (uint32_t i = 0; i < wavHeader.dataBodySize; i++) sampleData[i] = ((float) buffer[i]) / (pow(2, wavHeader.bitsPerSample - 1) - 1);
    }
    else if (wavHeader.bitsPerSample == 16) {
        int16_t* buffer;
        buffer = new int16_t[wavHeader.dataBodySize];
        data.read((char*)buffer, wavHeader.dataBodySize);
        data.close();
        sampleData = new float[wavHeader.dataBodySize];
        for (uint32_t i = 0; i < wavHeader.dataBodySize; i++) sampleData[i] = ((float) buffer[i]) / (pow(2, wavHeader.bitsPerSample - 1) - 1);
    }    
}

void WavFile::writeSampleData(std::string outputFilename) {
    std::fstream out (outputFilename, std::ios::binary | std::ios::out);
    if (out.is_open()) {
        if (wavHeader.bitsPerSample == 8) {
            int8_t* buffer;
            buffer = new int8_t[wavHeader.dataBodySize];
            for (uint32_t i = 0; i < wavHeader.dataBodySize; i++) buffer[i] = (int8_t) (sampleData[i] * pow(2, wavHeader.bitsPerSample - 1) - 1);
            out.write((char*)&wavHeader, sizeof(wavHeader));
            out.write((char*)buffer, wavHeader.dataBodySize);
            out.close();
            std::cout << "\nFile saved successfully\n";
        }
        else if (wavHeader.bitsPerSample == 16) {
            int16_t* buffer;
            buffer = new int16_t[wavHeader.dataBodySize];
            for (uint32_t i = 0; i < wavHeader.dataBodySize; i++) buffer[i] = (int16_t) (sampleData[i] * pow(2, wavHeader.bitsPerSample - 1) - 1);
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