#ifndef GAMMA_V2_H
#define GAMMA_V2_H

#include <stdint.h>
#include <stdlib.h>
#include <math.h>

void convertToGrayscale(const uint8_t* img, size_t width, size_t height, float a, float b, float c, uint8_t* first_img);
void applyGammaCorrection(uint8_t* first_img, size_t width, size_t height, float gamma, uint8_t* result);
void gamma_V2(const uint8_t* img, size_t width, size_t height, float a, float b, float c, float gamma, uint8_t* result);

#endif  // GAMMA_KORREKTUR_H3