#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "parse.h"
#include "read.h"



void skip_spaces(FILE* file) {
     int c;

    while ((c = fgetc(file)) == ' ' || c == '\t' || c == '\r' || c == '\n') {
    // Continue reading characters until a non-whitespace character is encountered
}
if (c != EOF) {
     ungetc(c, file);
}
}

void skip_comments(FILE* file) {
    int ch;
    do {
        while ((ch = fgetc(file)) == '#') {
            // If a comment is found, read until the end of the line
            while ((ch = fgetc(file)) != '\n' && ch != EOF) {
                // Continue reading characters until a newline or end of file is encountered
            }
        }
        // Move the file pointer back by one character if it's not the end of file
        if (ch != EOF) {
            ungetc(ch, file);
        }
    } while (ch == '#');
}

//Param 1 : name of the file
// Function to read P6 format PPM image from a file
PPMImage read_p6(const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        perror("Error opening file");                 //Opening the file we want to read from
        exit(EXIT_FAILURE);
    }
    skip_comments(file);

    char magic[3];
    if (fscanf(file, "%2s", magic) == 1) {           //Reading the header of the P6 file
        
    } else {
        fprintf(stderr,"Error reading from file\n");   
        exit(EXIT_FAILURE);
    }
    
    skip_comments(file);
    

    if (magic[0] != 'P' || magic[1] != '6') {
        fprintf(stderr, "Invalid PPM format\n");     //Checking the header if it is right or not
        exit(EXIT_FAILURE);
    }

    PPMImage ppmImage;
    skip_spaces(file);
    skip_comments(file);
    int temp_width, temp_height;
    if (fscanf(file, "%d %d", &temp_width, &temp_height) == 2) {         //Reading the width and height 
        
    } else {
        fprintf(stderr,"Error reading from file\n");       
        exit(EXIT_FAILURE);
    }

    // Check if width or height is negative
    if (temp_width <= 0 || temp_height <= 0) {
        fprintf(stderr, "Error: Width and height cannot be negative or zero.\n");  
        fclose(file); // Close the file before exiting
        exit(EXIT_FAILURE);
    }

    ppmImage.width = (size_t)temp_width;
    ppmImage.height = (size_t)temp_height;

     skip_spaces(file);
    skip_comments(file);

     int max_val;                               
    if (fscanf(file, "%d", &max_val) == 1) { // reading max value
    // Check if max_val is within the valid range (0 to 255)
    if (max_val < 0 || max_val > 255) {
        fprintf(stderr, "Error: Invalid max value. Must be in the range 0 to 255.\n");
        exit(EXIT_FAILURE);
    }
} else {
    fprintf(stderr,"Error reading or invalid max value\n");
    exit(EXIT_FAILURE);
}
    

    fgetc(file); // Read newline character

    skip_comments(file);

    ppmImage.image = (uint8_t*)malloc(sizeof(uint8_t) * ppmImage.width * ppmImage.height * 3);    //allocating memory for the pixels of the image
    if (!ppmImage.image) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }


    size_t elements_read = fread(ppmImage.image, sizeof(uint8_t), ppmImage.width * ppmImage.height * 3, file);    

    if (elements_read == (size_t)(ppmImage.width * ppmImage.height * 3)) {
        // Read successful
    } else {
        fprintf(stderr,"Error reading from file\n");           
        exit(EXIT_FAILURE);
    }
    
   

    fclose(file);

     for (size_t i = 0; i < ppmImage.width * ppmImage.height * 3; i++) {                
        if (ppmImage.image[i] > max_val) {
             // Check if every pixel is smaller or the same as the max value given
            fprintf(stderr, "Error: Pixel value exceeds the maximum value of 255\n");
            free(ppmImage.image); // Free allocated memory before exiting
            exit(EXIT_FAILURE);
        }
    }

    

    return ppmImage;
}
