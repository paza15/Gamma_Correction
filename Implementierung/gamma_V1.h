#ifndef GAMMA_V1_H
#define GAMMA_V1_H

#include <stdint.h>
#include <stdlib.h>
#include <math.h>
double integerPower(double base, int exponent);
double fractionalPower(double base, double fractionalPart);
double power(double base, double exponent);

void gamma_V1(const uint8_t* img, size_t width, size_t height, float a, float b, float c, float gamma, uint8_t* result);

#endif  // GAMMA_CORRECTION_H
