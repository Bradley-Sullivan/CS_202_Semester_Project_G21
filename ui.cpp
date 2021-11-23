#include "ui.h"

int UI::startMenu() {
    int selection;
    std::cout << "\n\n===========================================" << std::endl;
    std::cout << "=          WAV FILE EFFECT-ER 3000        =" << std::endl;
    std::cout << "===========================================" << std::endl;
    std::cout << "\n   (1) Open a file to modify\n   (2) Exit\n" << std::endl;
    do {
        std::cout << "> ";
        std::cin >> selection;
        if (selection != 1 && selection != 2) std::cout << "\nInvalid input. Try again." << std::endl;
    } while (selection != 1 && selection != 2);
    switch (selection) {
        case 1:
            return 1;
            break;
        case 2:
            std::cout << "\nGoodbye...\n" << std::endl;
            return 2;
            break;
        default: return 2;
    }
}

std::string UI::requestFilename() {
    std::string filename;
    std::cout << "Please enter the name of your audio file (ex. audioFile.wav)" << std::endl;
    std::cout << "> ";
    std::cin >> filename;
    return filename;
}

void UI::printWavMetadata(std::string filename, WAV_HEADER wavHeader) {
    std::cout << "\n=============================================" << std::endl;
    std::cout << "                   METADATA" << std::endl;
    std::cout << "Filename              : " << filename << std::endl;
    std::cout << "Samples per Second    : " << wavHeader.samplesPerSecond << std::endl;
    std::cout << "Bits per Sample       : " << wavHeader.bitsPerSample << std::endl;
    std::cout << "Number of Channels    : " << wavHeader.numChannels;
    if (wavHeader.numChannels == 2) std::cout << " (Stereo)" << std::endl;
    else if (wavHeader.numChannels == 1) std::cout << " (Mono)" << std::endl;
    std::cout << "Audio Duration        : " << wavHeader.dataBodySize / ((wavHeader.samplesPerSecond * wavHeader.bitsPerSample * wavHeader.numChannels) / 8) << " seconds" << std::endl;
    std::cout << "=============================================" << std::endl;
}

int UI::processorMenu() {
    char selection;
    bool help = false;
    do {
        if (help) {
            std::cout << "Available Effects" << std::endl;
            std::cout << "=================" << std::endl;
            std::cout << "   (1) Normalization - Maximizes the amplitude of the final waveform in proportion to the largest sample\n";
            std::cout << "   (2) Echo - Scales and shifts the waveform\n";
            std::cout << "   (3) Gain Normalization - Samples are multiplied by a scaling factor\n\t\t\t    which raises or lowers the overall amplitude of the waveform\n" << std::endl;
            std::cout << "> ";
        }
        else {
            std::cout << "Available Effects (Enter 'h' for a description of each effect)" << std::endl;
            std::cout << "==============================================================" << std::endl;
            std::cout << "   (h) Help\n   (1) Normalization\n   (2) Echo\n   (3) Gain Adjustment\n" << std::endl;
            std::cout << "> ";
        }
        std::cin >> selection;
        if (selection == 'h' || selection == 'H') help = true;
        else if (selection != '1' && selection != '2' && selection != '3' && selection != 'h') std::cout << "Invalid selection. Try again\n" << std::endl;
    } while(selection != '1' && selection != '2' && selection != '3');
    switch (selection) {
        case '1':
            return 1;
        case '2':
            return 2;
        case '3':
            return 3;
        default: break;
    }
    return -1;
}

std::string UI::requestOutputFilename() {
    std::string filename;
    std::cout << "Please enter the name of the output file (ex. outpuFile.wav)" << std::endl;
    std::cout << "> ";
    std::cin >> filename;
    return filename;
}