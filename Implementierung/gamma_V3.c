#include <emmintrin.h> 
#include <smmintrin.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "gamma_V3.h"

/**
 * Applies gamma correction to an image using SIMD instructions (Streaming SIMD Extensions). This function first converts the image to grayscale by calculating a weighted sum of the RGB channels, taking into account the relative sensitivity of the human eye to different colors. It then applies gamma correction to the grayscale values.
 *
 * @param img Pointer to the input image.
 * @param width The width of the image in pixels.
 * @param height The height of the image in pixels.
 * @param a The weighting coefficient for the R-channel in the grayscale conversion.
 * @param b The weighting coefficient for the G-channel in the grayscale conversion.
 * @param c The weighting coefficient for the B-channel in the grayscale conversion.
 * @param gamma The gamma value for gamma correction.
 * @param result Pointer to the memory area where the result image will be stored. This memory must already be allocated and have enough space for an image of the same size as the input image.
 *
 * The function processes four pixels at a time to improve performance through parallelization. Gamma correction is performed at the scalar level for each element in the vector. 
    For areas of the image that cannot be divided into groups of four pixels (e.g., at the edge of the image), gamma correction is calculated individually for each pixel.
 */


void gamma_V3(const uint8_t* img, size_t width, size_t height, float a, float b, float c, float gamma, uint8_t* result) {
    __m128 va = _mm_set1_ps(a);
    __m128 vb = _mm_set1_ps(b);
    __m128 vc = _mm_set1_ps(c);
    __m128 vsum = _mm_set1_ps(a + b + c);
    
    size_t restWidth = 0;
    size_t restX = 0;
        
    size_t processedWidth = width - (width % 4); // Width interpretation as group of four


    for(size_t y = 0; y < height; y++) {
        // restWidth >= 6 is a restriction to avoid memory leakage; 
        // The minimum remaining width length must be >= 5,3 (16 Byte min width length) 
        restWidth = width;
        for(size_t x = 0; x < processedWidth && restWidth >= 6; x += 4, restWidth -= 4, restX = x ) { // Processing four pixels
            size_t idx = (y * width + x) * 3;
           
           // For TESTING
           // printf("width = %u, x = %u, processedWidth = %u, restWidth = %u\n", width, x, processedWidth, restWidth);
            
            
            
            // Load 4 RGB pixel into SIMD register
            __m128i rgb = _mm_loadu_si128((__m128i*)(img + idx));

            //ES WIRD IN BIG ENDIAN GESPEICHERT!!!
            
            //Shuffle mask for gathering 4 R elements, 4 G elements and 4 B elements (also set last 4 elements to duplication of first 4 elements).
            const __m128i shuffle_mask = _mm_set_epi8(9,6,3,0, 11,8,5,2, 10,7,4,1, 9,6,3,0);

            __m128i r3_r2_r1_r0_b3_b2_b1_b0_g3_g2_g1_g0_r3_r2_r1_r0 = _mm_shuffle_epi8(rgb, shuffle_mask);

            // shifte 12 byte nach rechts
            __m128i R = _mm_slli_si128(r3_r2_r1_r0_b3_b2_b1_b0_g3_g2_g1_g0_r3_r2_r1_r0,12);
            // nachgeschobene Byte auf 0 setzen
            R = _mm_srli_si128(R, 12);
            
            //__m128i maskG = _mm_set_epi8(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1); // -1 entspricht 0xFF
            __m128i G = _mm_srli_si128(_mm_slli_si128(r3_r2_r1_r0_b3_b2_b1_b0_g3_g2_g1_g0_r3_r2_r1_r0, 8),12); 
            //G =_mm_and_si128(G, maskG);
    

            __m128i B = _mm_srli_si128(_mm_slli_si128(r3_r2_r1_r0_b3_b2_b1_b0_g3_g2_g1_g0_r3_r2_r1_r0,4),12);

            //R = _mm_unpackhi_epi8(R, _mm_setzero_si128());
            // Zuerst erweitern wir die 8-Bit-Werte zu 16-Bit-Werten
            //__m128i R16 = _mm_unpacklo_epi8(R, _mm_setzero_si128());

            // Nullen am Ende entfernen
            //R16 = _mm_srli_si128(R16, 8);

            // Dann erweitern wir die 16-Bit-Werte zu 32-Bit-Werten
            //__m128i R32 = _mm_unpacklo_epi16(R16, _mm_setzero_si128());
            R = _mm_cvtepu8_epi32(R);
            G = _mm_cvtepu8_epi32(G);
            B = _mm_cvtepu8_epi32(B);

            __m128 Rf = _mm_cvtepi32_ps(R);
            __m128 Gf = _mm_cvtepi32_ps(G);
            __m128 Bf = _mm_cvtepi32_ps(B);

            // Convert gray scale 
            __m128 gray = _mm_div_ps(_mm_add_ps(_mm_add_ps(_mm_mul_ps(va, Rf), _mm_mul_ps(vb, Gf)), _mm_mul_ps(vc, Bf)), vsum);
        //   printf("gray: ");
        //  print_m128(gray);
            // Applying the gammacorrection for eacht element in the vector on a scalaric level
            for (int i = 0; i < 4; i++) {
                float grayValue = ((float*)&gray)[i];
                ((float*)&gray)[i] = powf(grayValue * 1.0f / 255.0f, gamma) * 255.0f;
            }

            __m128i corrected_gray8bit = _mm_cvtps_epi32(gray);
            result[y * width + x] =(uint8_t) _mm_extract_epi32(corrected_gray8bit, 0);
            result[y * width + x + 1] =(uint8_t) _mm_extract_epi32(corrected_gray8bit, 1);
            result[y * width + x + 2] =(uint8_t) _mm_extract_epi32(corrected_gray8bit, 2);
            result[y * width + x + 3] =(uint8_t) _mm_extract_epi32(corrected_gray8bit, 3);

        }
        if((width % 4) == 0 && restWidth < 6){
            // Edge cases handling -> when width IS div by 4 but restWidth < 6
            for (size_t x = restX; x < width; x++) {
                size_t idx = (y * width + x) * 3;

                // Each pixel scalaric calculate
                float R = img[idx];
                float G = img[idx + 1];
                float B = img[idx + 2];

                // Gray scale convertion
                float gray = (a * R + b * G + c * B) / (a + b + c);

                // Gammacorrektur
                float corrected_gray = powf(gray / 255.0f, gamma) * 255.0f;
                result[y * width + x] = (uint8_t)corrected_gray;
            }
        }
        else{
            // Edge cases handling -> when width is not div by 4
            for (size_t x = processedWidth; x < width; x++) {
                size_t idx = (y * width + x) * 3;

                // Each pixel scalaric calculate
                float R = img[idx];
                float G = img[idx + 1];
                float B = img[idx + 2];

                // Gray scale convertion
                float gray = (a * R + b * G + c * B) / (a + b + c);

                // Gammacorrektur
                float corrected_gray = powf(gray / 255.0f, gamma) * 255.0f;
                result[y * width + x] = (uint8_t)corrected_gray;
            }
        }
    }
}      
