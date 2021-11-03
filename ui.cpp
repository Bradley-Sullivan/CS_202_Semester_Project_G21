#include "ui.h"

int UI::startMenu() {
    int selection;
    std::cout << "\n\n===========================================" << std::endl;
    std::cout << "=          WAV FILE EFFECT-ER 3000        =" << std::endl;
    std::cout << "===========================================" << std::endl;
    std::cout << "\n   (1) Open a file to modify\n   (2) Exit\n" << std::endl;
    std::cout << "> ";
    std::cin >> selection;
    switch (selection) {
        case 1:
            do {
                requestFilename();
            } while (audioFile.loadWavData(filename) == -1);
            printWavMetadata();
            processorMenu();
            break;
        case 2:
            std::cout << "\nGoodbye...\n" << std::endl;
            return -1;
            break;
        default:
            std::cout << "\nInvalid input. Try again.\n" << std::endl;
            break;
    }
    return 0;
}

void UI::requestFilename() {
    std::cout << "Please enter the name of your audio file (ex. audioFile.wav)" << std::endl;
    std::cout << "> ";
    std::cin >> filename;
}

void UI::printWavMetadata() {
    header = audioFile.getWavHeader();
    std::cout << "\n\n=============================================" << std::endl;
    std::cout << "                   METADATA" << std::endl;
    std::cout << "Filename              : " << filename << std::endl;
    std::cout << "Samples per Second    : " << header.samplesPerSecond << std::endl;
    std::cout << "Bits per Sample       : " << header.bitsPerSample << std::endl;
    std::cout << "Number of Channels    : " << header.numChannels;
    if (header.numChannels == 2) std::cout << " (Stereo)" << std::endl;
    else if (header.numChannels == 1) std::cout << " (Mono)" << std::endl;
    std::cout << "=============================================\n\n" << std::endl;
}

void UI::processorMenu() {
    char selection;
    bool help = false;
    Effects addEffect(audioFile.getSampleData(), header.dataBodySize, header.bitsPerSample, header.samplesPerSecond);
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
            addEffect.normalize();
            break;
        case '2':
            addEffect.echo();
            break;
        case '3':
            double gainFactor;
            do {
                std::cout << "Please enter your gain adjustment factor (between 0 and 3)\n> ";
                std::cin >> gainFactor;
                if (gainFactor < 0 || gainFactor > 3) std::cout << "Invalid factor entered. Try again\n" << std::endl;
            } while (gainFactor < 0 || gainFactor > 3);
            addEffect.gainAdjustment(gainFactor);
            break;
        default:
            std::cout << "you weren't supposed to get here...." << std::endl;
            break;
    }
    requestOutputFilename();
}

void UI::requestOutputFilename() {
    std::cout << "Please enter the name of the output file (ex. outpuFile.wav)" << std::endl;
    std::cout << "> ";
    std::cin >> outputFilename;
    audioFile.saveFile(outputFilename);
}