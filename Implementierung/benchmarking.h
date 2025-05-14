#ifndef BENCHMARK_H
#define BENCHMARK_H

#include <stdint.h>
#include <time.h>

// Define the function prototype for benchmarking
double benchmarking(uint32_t rep, int version, const uint8_t *img, size_t width, size_t height, float a, float b, float c, float gamma, uint8_t *result);

#endif // BENCHMARK_H
