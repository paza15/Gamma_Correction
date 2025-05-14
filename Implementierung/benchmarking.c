#define _POSIX_C_SOURCE 199309L
#include "gamma_V0.h"
#include "gamma_V1.h"
#include "gamma_V2.h"
#include "gamma_V3.h"
#include "gamma_V4.h"
#include <time.h>
#include "benchmarking.h"
#include <stdio.h>

// Helper function to prevent the compiler from optimizing away the loop

static void escape(void *p){
    __asm__ volatile ("" : : "g"(p): "memory");     // Use inline assembly to ensure the compiler doesn't remove the loop during optimization
 
}

// Function to benchmark different gamma correction versions

double benchmarking(uint32_t rep, int  version, const uint8_t* img, size_t width, size_t height, float a, float b, float c, float gamma, uint8_t* result){
 struct timespec start, end;
    
    
   
   
    // Choose the appropriate gamma correction version based on the provided 'version' argument

    switch (version) {
        case 0:
           clock_gettime(CLOCK_MONOTONIC, &start);
            for (uint32_t j = 0; j < rep; j++) {
                escape(result);   // Ensure enough runtime between time measurements
                gamma_V0(img,width,height,a,b,c,gamma,result);
                escape(result);
            }
            clock_gettime(CLOCK_MONOTONIC, &end);
            break;
        case 1:
            clock_gettime(CLOCK_MONOTONIC, &start);
            for (uint32_t j = 0; j < rep; j++) {
                  escape(result);
                gamma_V1(img,width,height,a,b,c,gamma,result);
                escape(result);
            }
            clock_gettime(CLOCK_MONOTONIC, &end);
            break;
        case 2:
            clock_gettime(CLOCK_MONOTONIC, &start);
            for (uint32_t j = 0; j < rep; j++) {
                 escape(result);
                gamma_V2(img,width,height,a,b,c,gamma,result);
                escape(result);
            }
            clock_gettime(CLOCK_MONOTONIC, &end);
            break;
        case 3:
         clock_gettime(CLOCK_MONOTONIC, &start);
            for (uint32_t j = 0; j < rep; j++) {
                 escape(result);
                gamma_V3(img,width,height,a,b,c,gamma,result);
                escape(result);
            }
            clock_gettime(CLOCK_MONOTONIC, &end);
            break;
            case 4:
            clock_gettime(CLOCK_MONOTONIC, &start);
            for (uint32_t j = 0; j < rep; j++) {
                  escape(result);
                gamma_V4(img,width,height,a,b,c,gamma,result);
                escape(result);
            }
            clock_gettime(CLOCK_MONOTONIC, &end);
            break;
             
        default:
            fprintf(stderr,"Invalid version\n");
            free((void*)img);  // Free allocated memory before exiting in case of an invalid version
            free((void*)img);
free((void*)result);
             
        exit(EXIT_FAILURE);
    }
      // Calculate and return the time taken for benchmarking
    return (end.tv_sec - start.tv_sec) + 1e-9 * (end.tv_nsec - start.tv_nsec);
}
