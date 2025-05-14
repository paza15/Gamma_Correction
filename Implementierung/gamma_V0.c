#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "gamma_V0.h"

//This is the implementation realised with the methods from the library math.h to calculate the exponential function

void gamma_V0(const uint8_t* img, size_t width, size_t height,float a, float b, float c,float gamma,uint8_t* result){
    //iterating through the 'array' of pixels and for each of them applying the gamma-korrektur algorithm as described 
    for(size_t i = 0; i < height; i++){
        for(size_t j = 0; j < width; j++){

            size_t position = (i * width + j) * 3; //the starting index for each pixel
            float R = img[position]; //the red color
            float G = img[position + 1]; //the green color
            float B = img[position + 2]; //the blue color

            //converting in grayscale first 
            float d = (a * R + b * G + c * B) / (a + b + c); 
            
            //gamma correction
            float result_1 = powf(d/255.0f, gamma)* 255.0f; //using function powf from math.h library

            uint8_t pixel_val = (uint8_t)fminf(fmax(result_1,0), 255);

            //recreating the result file from the input 
            result[i * width + j] = pixel_val;
}
}
}