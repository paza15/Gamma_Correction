#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "parse.h"
#include "read.h"
#include "write.h"
#include <string.h>

void write_p5(const char* filename, uint8_t* image, size_t width, size_t height) {
    // Create a buffer to store the updated filename
    char updatedFilename[strlen(filename) + 5];  // ".ppm" has 4 characters, plus 1 for null-terminator

    // Copy the original filename to the buffer
    strcpy(updatedFilename, filename);

    // Append ".ppm" to the filename
    strcat(updatedFilename, ".pgm");

    FILE* file = fopen(updatedFilename, "wb");
    if (!file) {
        perror("Error opening file");
        free(image);      //Opening the file
        exit(EXIT_FAILURE);
    }

    // Write P5 header widht and height and max value 255
    fprintf(file, "P5\n%ld %ld\n255\n", width, height);    

    // Write the pixel values in binary mode
    for (size_t i = 0; i < height; i++) {
        for (size_t j = 0; j < width; j++) {
            fwrite(&image[i * width + j], sizeof(uint8_t), 1, file);
        }
    }


    fclose(file);
}
