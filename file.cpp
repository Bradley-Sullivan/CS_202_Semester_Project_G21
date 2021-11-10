#include "file.h"

int16_t* WavFile::getSampleData() const { return sampleData; }

WAV_HEADER WavFile::getWavHeader() const { return wavHeader; }

double WavFile::getAudioDuration() const { return (double) (wavHeader.dataBodySize / ((wavHeader.samplesPerSecond * wavHeader.bitsPerSample * wavHeader.numChannels) / 8)); }

int WavFile::loadWavData(std::string filename) {
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

        //Points the sampleData pointer to an array allocated on the heap as we don't want to overflow the stack if the wav file is too large
        sampleData = new int16_t[wavHeader.dataBodySize];

        //Reads in the sample data into the buffer
        wavFile.read((char*)sampleData, wavHeader.dataBodySize);
        wavFile.close();
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
    if (outFile.is_open()) {
        outFile.write((char*)&wavHeader, sizeof(wavHeader));
        outFile.write((char*)sampleData, wavHeader.dataBodySize);
        outFile.close();
        std::cout << "\nFile save successfully";
        delete sampleData; 
    }
    else {
        std::cout << "\nError opening output file to save sample data. Could not save changes.\n" << std::endl;
    }
}