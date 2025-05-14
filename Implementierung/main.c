#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "read.h"
#include "parse.h"
#include "write.h"
#include "gamma_V0.h"
#include "gamma_V1.h"
#include "gamma_V2.h"
#include "gamma_V3.h"
#include "gamma_V4.h"
#include "benchmarking.h"

int main(int argc, char **argv){
    uint8_t* result;

    struct arg d = {                     //creating the actual struct we use
        0,          //defaault implementation
        1,          //default iterations
        "output",    //default output name
        0.5f,       //default gama
        0.299f,     //default a
        0.587f,     //default b
        0.114f,     //default c
        NULL,       
        0,
        0,
    };

    parse(&d, argc, argv);           //getting all the arguments from the user and parsing them

    result = (uint8_t*)malloc(sizeof(uint8_t) * d.width * d.height);   //allocation for result

    double time = benchmarking(d.B,d.V,d.image,d.width,d.height,d.c1,d.c2,d.c3,d.gamma,result);  
    printf("The time is: %lf \n",time);      //benchmark tests and running the programm 
    
    free(d.image);
    write_p5(d.o,result,d.width,d.height);       //writing the result and doing the frees needed to avoid memory leaks
    free(result);
    printf("The version used is version number %d. \n",d.V);
    printf("You have done %d iterations. \n", d.B);
    printf("Your values for a b and c are : a = %f , b = %f, c = %f and the value of your gamma is %f. \n", d.c1, d.c2, d.c3, d.gamma);
    printf("The output name is %s. \n", d.o);
    return 0;
}

