/**
 * @file ui.h
 * @author Bradley Sullivan, Emily Cuellar, Ever McNeill
 * @brief Header file for the UI class which handles printing to console and collecting menu inputs
 * @date 2021-11-30
 * 
 */

#ifndef UI_H
#define UI_H

#include <iostream>
#include <string>
#include "file.h"
#include "effects.h"

class UI {
    WAV_HEADER header;
    public:
        /**
         * @brief Displays program start menu to console and returns user selection to calling function.
         * 
         * @return int 
         */
        int startMenu();

        /**
         * @brief Prompts the user for the name of the input wav file.
         * 
         * @return std::string 
         */
        std::string requestFilename();

        /**
         * @brief Displays information about the input wav file
         * 
         */
        void printWavMetadata(std::string, WAV_HEADER);

        /**
         * @brief Displays the processor menu to the console and returns user selection to calling function.
         * 
         * @return int 
         */
        int processorMenu();

        /**
         * @brief Requests the name of the output file of which the modified sample data will be written to.
         * 
         * @return std::string 
         */
        std::string requestOutputFilename();
};

#endif //UI_H