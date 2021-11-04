#include "file.h"

int16_t* WavFile::getSampleData() const { return sampleData; }

WAV_HEADER WavFile::getWavHeader() const { return wavHeader; }

double WavFile::getAudioDuration() const { return (double) (wavHeader.dataBodySize / ((wavHeader.samplesPerSecond * wavHeader.bitsPerSample * wavHeader.numChannels) / 8)); }

//Need to position file pointer to the beginning of the sample data within the data chunk
//Otherwise, if the wav file isn't exactly in the form that we want we just read in garbage data
int WavFile::loadWavData(std::string filename) {
    uint8_t chunkID[4];
    uint32_t chunkSize;
    std::cout << "\nLoading file data...";
    std::ifstream wavFile (filename, std::ios::binary | std::ios::in);
    if (wavFile.is_open()) {
        wavFile.read((char*)&wavHeader, sizeof(wavHeader) - 8);
        do {
            wavFile.read((char*)chunkID, sizeof(chunkID));
            wavFile.read((char*)&chunkSize, sizeof(chunkSize));
            if (chunkID[0] != 'd' && chunkID[1] != 'a' && chunkID[2] != 't' && chunkID[3] != 'a') wavFile.seekg(chunkSize + wavFile.tellg());
        } while (chunkID[0] != 'd' && chunkID[1] != 'a' && chunkID[2] != 't' && chunkID[3] != 'a');
        for (int i = 0; i < 4; i++) wavHeader.dataID[i] = chunkID[i];
        wavHeader.dataBodySize = chunkSize;
        sampleData = new int16_t[wavHeader.dataBodySize];
        wavFile.read((char*)sampleData, wavHeader.dataBodySize);
        wavFile.close();
        if (wavHeader.riffID[0] != 'R' && wavHeader.riffID[1] != 'I' && wavHeader.riffID[2] != 'F' && wavHeader.riffID[3] != 'F') {
            std::cout << "File incorrectly formatted. Program requires RIFF .wav files." << std::endl;
            return -1;
        }
        if (wavHeader.formatCode[0] != 'W' && wavHeader.formatCode[1] != 'A' && wavHeader.formatCode[2] != 'V' && wavHeader.formatCode[3] != 'E') {
            std::cout << "File incorrectly formatted. Program requires .wav files with fmt code WAVE." << std::endl;
            return -1;
        }
        if (wavHeader.fmtCode != 1) {
            std::cout << "File incorrectly formatted. Program requires PCM encoded .wav files (i.e. fmt code = 1)." << std::endl;
            return -1;
        }
        if (wavHeader.numChannels > 2) {
            std::cout << "File contains more than 2 audio channels. Program will only accept Mono or Stereo .wav files." << std::endl;
            return -1;
        }
        if (wavHeader.bitsPerSample > 16) {
            std::cout << "File contains a bit-depth greater than 16-bit. Program requires 8 to 16 bit .wav files" << std::endl;
            return -1;
        }
    }
    else {
        std::cout << "Unable to open " << filename << " . File does not exist or is unreadable\n" << std::endl;
        return -1;
    }
    std::cout << "\nFile loaded successfully.\n" << std::endl;
    return 0;
}

void WavFile::saveFile(std::string outputFilename) {
    std::fstream outFile;
    outFile.open(outputFilename, std::ios::out | std::ios::binary);
    outFile.write((char*)&wavHeader, sizeof(wavHeader));
    outFile.write((char*)sampleData, wavHeader.dataBodySize);
    outFile.close();
    std::cout << "\nFile save successfully";
    delete sampleData;
}