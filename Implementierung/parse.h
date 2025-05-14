#ifndef PARSE_H
#define PARSE_H

#include <stdint.h>
#include <stddef.h>


struct arg {
    uint32_t V;
    uint32_t B;
    char* o;
    float gamma;
    float c1;
    float c2;
    float c3; // this is in order to stop in case of printing 
    uint8_t* image;
    size_t height;
    size_t width;
};

void parse(struct arg* parser, int argc, char** argv);

#endif // PARSE_H
