#include <iostream>
#include <string>
#include "file.h"
#include "ui.h"
#include "effects.h"

int main() {
    UI ui;
    WavFile audioFile;
    Effects x;
    
    std::string filename, outputFilename;
    int selection;
    do {
        selection = ui.startMenu();
        switch (selection) {
            case 1:
                do { filename = ui.requestFilename(); } while (audioFile.loadWavHeader(filename) == -1);
                std::cout << "\nHeader loaded successfully." << std::endl;
                ui.printWavMetadata(filename, audioFile.getWavHeader());
                audioFile.loadSampleData(filename);
                std::cout << "\nSample Data loaded successfully.\n" << std::endl;
                switch (ui.processorMenu()) {
                    case 1:
                        x.normalize(audioFile.getSampleData(), audioFile.getWavHeader());                        
                        break;
                    case 2:
                        x.echo(audioFile.getSampleData(), audioFile.getWavHeader());
                        break;
                    case 3:
                        x.gainAdjustment(audioFile.getSampleData(), audioFile.getWavHeader());                                
                        break;
                    default:
                        std::cout << "Something went wrong...\n" << std::endl;
                        return 0;
                }
                outputFilename = ui.requestOutputFilename();
                audioFile.writeSampleData(outputFilename);
            default: break;
        }
    } while (selection == 1);

    return 0;
}