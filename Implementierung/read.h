#ifndef PPM_READER_H
#define PPM_READER_H

#include <stdint.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

typedef struct {
    size_t width;
    size_t height;
    uint8_t* image;
} PPMImage;

PPMImage read_p6(const char* filename);

#endif // PPM_READER_H
