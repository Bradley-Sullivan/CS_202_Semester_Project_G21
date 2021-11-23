#include <iostream>
#include <string>
#include "file.h"
#include "ui.h"


int main() {
    UI ui;
    WavFile audioFile;
    
    std::string filename, outputFilename;
    int selection;
    do {
        selection = ui.startMenu();
        switch (selection) {
            case 1:
                do { filename = ui.requestFilename(); } while (audioFile.loadWavHeader(filename) == -1);
                std::cout << "\nHeader loaded successfully." << std::endl;
                ui.printWavMetadata(filename, audioFile.getWavHeader());
                switch (audioFile.getWavHeader().bitsPerSample) {
                    case 8:
                    {
                        audioFile.loadSampleData(filename);
                        std::cout << "\nSample Data loaded successfully.\n" << std::endl;
                        Effects addEffect(audioFile.getSampleData(), audioFile.getWavHeader());
                        switch (ui.processorMenu()) {
                            case 1:
                                addEffect.normalize();
                                break;
                            case 2:
                                addEffect.echo(audioFile.getAudioDuration());
                                break;
                            case 3:
                                addEffect.gainAdjustment();
                                break;
                            default:
                                std::cout << "Something went wrong...\n" << std::endl;
                                return 0;
                        }
                        outputFilename = ui.requestOutputFilename();
                        audioFile.writeSampleData(outputFilename);
                        break;
                    }
                    case 16:
                    {
                        audioFile.loadSampleData(filename);
                        std::cout << "\nSample Data loaded successfully.\n" << std::endl;
                        Effects addEffect(audioFile.getSampleData(), audioFile.getWavHeader());
                        switch (ui.processorMenu()) {
                            case 1:
                                addEffect.normalize();
                                break;
                            case 2:
                                addEffect.echo(audioFile.getAudioDuration());
                                break;
                            case 3:
                                addEffect.gainAdjustment();
                                break;
                            default:
                                std::cout << "Something went wrong...\n" << std::endl;
                                return 0;
                        }
                        outputFilename = ui.requestOutputFilename();
                        audioFile.writeSampleData(outputFilename);
                        break;
                    }
                }

            case 2:
                break;
        }
    } while (selection == 1);

    return 0;
}