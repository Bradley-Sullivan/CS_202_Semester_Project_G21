/**
 * @file effects.h
 * @author Bradley Sullivan, Emily Cuellar, Ever McNeill
 * @brief Header file for the Effects class which handles the modification of wav sample data
 * @date 2021-11-10
 * 
 */

#ifndef EFFECTS_H
#define EFFECTS_H
#include <iostream>
#include <math.h>
#include "file.h"

class Effects {
    public:
        /**
         * @brief Adds an echo/reverb effect to wav sample data.
         * 
         */
        void echo(double*, WAV_HEADER);

        /**
         * @brief Normalizes wav samples in relation to the largest sample.
         * 
         */
        void normalize(double*, WAV_HEADER);

        /**
         * @brief Scales every sample with a given gain factor.
         * 
         */
        void gainAdjustment(double*, WAV_HEADER);
};

#endif //EFFECTS_H