#ifndef GAMMA_V3_H
#define GAMMA_V3_H

#include <stdint.h>
#include <stdlib.h>
#include <math.h>

void gamma_V3(const uint8_t* img, size_t width, size_t height, float a, float b, float c, float gamma, uint8_t* result);

#endif  // GAMMA_KORREKTUR_SSE