#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "gamma_V2.h"

/* 
This implementation contains 2 helper functions:
1. convertToGrayscale - it converts an image and its pixels to grayscale only
2. applyGammaCorrection - it applies gamma correction on a grayscale image 
3. gamma_korrektur - is the "main" function  that calls the two helper functions mentioned above

In this implementation: Using the fact that a^b = pow(a,b) = exp(b * log(a)), using two
other mathematical functions from math.h to have diversity between implementations
*/

void convertToGrayscale(const uint8_t* img, size_t width, size_t height, float a, float b, float c, uint8_t* first_img){
    //iterating through the 'first_img' pixels and for each of them applying the grayscale formula as described 
for(size_t i = 0; i < height; i++){
    for(size_t j = 0; j < width; j++){

        size_t position = (i * width + j) * 3; //the starting index for each pixel
        float R = img[position]; //the red color
        float G = img[position + 1]; //the green color
        float B = img[position + 2]; //the blue color

//converting in grayscale  
float d = (a * R + b * G + c * B) / (a + b + c); 

uint8_t gray = (uint8_t)fminf(fmaxf(d,0), 255);

//Creating the grayscaled image from the input image 
first_img[i * width + j] = gray;

}
}
}


void applyGammaCorrection(uint8_t* first_img, size_t width, size_t height, float gamma,uint8_t* result){
    for(size_t i = 0; i < height; i++){
        for(size_t j = 0; j < width; j++){
            size_t position = (i * width + j);  //the starting index for each pixel
            float d = first_img[position]; // grayscale value 

            //Apply gamma correction
            float result_1 = expf(logf(d / 255.0f) * gamma) * 255.0f; 
            uint8_t correctedValue = (uint8_t)fminf(fmaxf(result_1,0), 255);

            //Creating the final result after applying the gamma correction to the grayscaled image 
            result[i * width + j] = correctedValue;
        }
    }
}

void gamma_V2(const uint8_t* img, size_t width, size_t height, float a, float b, float c, float gamma, uint8_t* result){

    //First, convert the img to grayscale
    convertToGrayscale(img, width, height, a, b, c, result);

    //Then, apply gamma correction on the grayscaled img 
    applyGammaCorrection(result, width, height, gamma,result);
}

