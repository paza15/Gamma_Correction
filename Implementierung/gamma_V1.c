#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>


//This is the implementation that uses basic mathematic operators to calculate the power 

double integerPower(double base, int exponent){

    /*
    In this function: a simple pow function if the exponent is an int/the whole number part of the given exponent
    The logic is multiplying the base by itself as many times as the exponent's value 
    */
    double result = 1.0; 
    for(int i = 0; i < exponent; ++i){
        result *= base;
    }
    return result;
}

double fractionalPower(double base, double fractionalPart){

   /*
   If the fractionalPart is close to 0.5: using linear interpolation for fractional parts less than 0.5
   This part uses a linear interpolation between 1 (= [any number]^0) and the base itself (= [any number]^1)
   */
    if(fractionalPart < 0.5){
        return 1 + fractionalPart * (base - 1);
    }

    /*
    Otherwise: using the Babylonian method/Heron's method to calculate square root
    This method is an iterative technique: starting with an initial guess of what the result may be and then iteratively perfecting
    this guess
    It repeatedly takes the average of the current estimate and the result of dividing the number by the current estimate
    The repetitions of 10 times has shown to provide a good approximation of the square root 
    */
    else{
        double squareRoot = base;
        for(int i = 0; i < 10; ++i){
        squareRoot = (squareRoot + base  / squareRoot) / 2; 
        }
        return squareRoot;
    }
}

double power(double base, double exponent){
    
    //this functioin is the main function that uses the two functions above as helper functions
    
    //declaring the exponent and defining it in 2 parts: the int part and the fractional part
    //so the number 3.567 would be divided into intPart = 3 and fractionalPart = 0.567
    int intPart = (int)exponent; 
    double fractionalPart = exponent - intPart;

    //calculating the result for the intPart using the first helper function 
    double intPow = integerPower(base,intPart > 0 ? intPart : -intPart);

    //calculating the result for the fractionalPart using the second helper function 
    double fractionalPow = fractionalPower(base, fractionalPart);

    //calculating and returning the final result by multiplying intPow and fractionalPow
    return intPow * fractionalPow;
}


void gamma_V1(const uint8_t* img, size_t width, size_t height,float a, float b, float c,float gamma,uint8_t* result){
    //iterating through the 'array' pixels and for each of them applying the gamma-korrektur algorithm as described 
    for(size_t i = 0; i < height; i++){
        for(size_t j = 0; j < width; j++){

            size_t position = (i * width + j) * 3; //the starting index for each pixel
            float R = img[position]; //the red color
            float G = img[position + 1]; //the green color
            float B = img[position + 2]; //the blue color

            //converting in grayscale first 
            float d = (a * R + b * G + c * B) / (a + b + c); 

            //gamma correction
            float result_1 = power(d/255.0f, gamma)* 255.0f; //using the power function above 

            uint8_t pixel_val = (uint8_t)fminf(fmaxf(result_1,0), 255);

            //recreating the result file from the input 
            result[i * width + j] = pixel_val;
            }
        }
}   