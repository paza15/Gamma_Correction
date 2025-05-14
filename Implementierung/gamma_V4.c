#include <emmintrin.h> 
#include <smmintrin.h>
#include <stdint.h>
#include "gamma_V4.h"

/*
 * Basic approximational approach of the natural logarithm using a Taylor series expansion.
 *
 * Parameters:
 *  - __m128 x: A 128-bit SIMD floating-point vector containing four single-precision floating-point elements.
 *
 * Returns:
 *  - __m128: A 128-bit SIMD floating-point vector containing the approximate natural logarithm of each element in `x`.
 *
 * Description:
 * This function implements a basic approximation of the natural logarithm function for each element in the input vector `x`,
 * utilizing a truncated Taylor series expansion: ln(1 + x) ≈ x - x^2/2 + x^3/3. 
 * The input vector `x` is assumed to be close to 1, as the accuracy of the approximation depends on the proximity of `x` 
 * to 1. This function is designed for performance in SIMD operations and may be used in applications where a high-precision
 * logarithm is not critical, but fast computation is required.
 */
__m128 log_approx(__m128 x){
    __m128 one = _mm_set1_ps(1.0f);
    __m128 x_minus_one = _mm_sub_ps(x, one);

    __m128 term1 = x_minus_one;
    __m128 term2 = _mm_div_ps(_mm_mul_ps(x_minus_one, x_minus_one), _mm_set1_ps(2.0f));
    __m128 term3 = _mm_div_ps(_mm_mul_ps(_mm_mul_ps(x_minus_one, x_minus_one), x_minus_one), _mm_set1_ps(3.0f));
    __m128 term4 = _mm_div_ps(_mm_mul_ps(_mm_mul_ps(_mm_mul_ps(x_minus_one, x_minus_one), x_minus_one), x_minus_one), _mm_set1_ps(4.0f));
    __m128 term5 = _mm_div_ps(_mm_mul_ps(_mm_mul_ps(_mm_mul_ps(_mm_mul_ps(x_minus_one, x_minus_one), x_minus_one), x_minus_one), x_minus_one), _mm_set1_ps(5.0f));
    __m128 term6 = _mm_div_ps(_mm_mul_ps(_mm_mul_ps(_mm_mul_ps(_mm_mul_ps(_mm_mul_ps(x_minus_one, x_minus_one), x_minus_one), x_minus_one), x_minus_one), x_minus_one), _mm_set1_ps(6.0f));

    // Sum and subtract terms accordingly
    return _mm_sub_ps(_mm_sub_ps(_mm_add_ps(_mm_add_ps(term1, term3), term5), term2), _mm_add_ps(term4, term6));
}

/*
 * Basic approximational approach of the exponential function using a Taylor series expansion.
 *
 * Parameters:
 *  - __m128 x: A 128-bit SIMD floating-point vector containing four single-precision floating-point elements.
 *
 * Returns:
 *  - __m128: A 128-bit SIMD floating-point vector containing the approximate exponential value of each element in `x`.
 *
 * Description:
 * This function provides an approximation of the exponential function for each element in the input vector `x`, using
 * a truncated Taylor series expansion: e^x ≈ 1 + x + x^2/2 + x^3/6. The function is intended for use in situations where
 * computational efficiency is prioritized over high precision. It leverages SIMD operations to compute the exponential
 * of four single-precision floating-point numbers simultaneously, making it suitable for performance-critical applications
 * that can tolerate approximation errors.
 */
__m128 exp_approx(__m128 x){
    __m128 one = _mm_set1_ps(1.0f);
    __m128 x_squared = _mm_mul_ps(x, x);
    __m128 x_cubed = _mm_mul_ps(x_squared, x);
    __m128 x_quartic = _mm_mul_ps(x_cubed, x);
    __m128 x_quintic = _mm_mul_ps(x_quartic, x); // x^5
    __m128 x_sextic = _mm_mul_ps(x_quintic, x); // x^6

    __m128 term1 = one;
    __m128 term2 = x;
    __m128 term3 = _mm_div_ps(x_squared, _mm_set1_ps(2.0f)); // 2! = 2
    __m128 term4 = _mm_div_ps(x_cubed, _mm_set1_ps(6.0f)); // 3! = 6
    __m128 term5 = _mm_div_ps(x_quartic, _mm_set1_ps(24.0f)); // 4! = 24
    __m128 term6 = _mm_div_ps(x_quintic, _mm_set1_ps(120.0f)); // 5! = 120
    __m128 term7 = _mm_div_ps(x_sextic, _mm_set1_ps(720.0f)); // 6! = 720

    // Sum all terms
    return _mm_add_ps(_mm_add_ps(_mm_add_ps(_mm_add_ps(_mm_add_ps(_mm_add_ps(term1, term2), term3), term4), term5), term6), term7);
}

/*
 * Applies gamma correction to an image using SIMD operations and approximation functions for logarithmic and exponential calculations.
 *
 * Parameters:
 *  - const uint8_t* img: Pointer to the input image data in 8-bit unsigned integer format.
 *  - size_t width: The width of the image in pixels.
 *  - size_t height: The height of the image in pixels.
 *  - float a, b, c: Coefficients for the weighted sum in grayscale conversion, reflecting the human eye's sensitivity to red, green, and blue.
 *  - float gamma: The gamma correction factor.
 *  - uint8_t* result: Pointer to the memory where the gamma-corrected grayscale image will be stored.
 *
 * Description:
 * This function converts an RGB image to grayscale using a weighted sum approach and then applies gamma correction using the specified
 * gamma value. The grayscale conversion takes into account the human eye's different sensitivities to red, green, and blue by using
 * the coefficients `a`, `b`, and `c`. The gamma correction is performed using an approximate logarithm and exponential functions
 * for efficiency. The function processes four pixels simultaneously using SIMD instructions to enhance performance. It is suitable for
 * applications requiring fast gamma correction where exact precision is not critical. The function handles edge cases where the image
 * width is not a multiple of four by processing the remaining pixels without SIMD instructions.
 */
void gamma_V4(const uint8_t* img, size_t width, size_t height, float a, float b, float c, float gamma, uint8_t* result) {
    __m128 va = _mm_set1_ps(a);
    __m128 vb = _mm_set1_ps(b);
    __m128 vc = _mm_set1_ps(c);
    __m128 vsum = _mm_set1_ps(a + b + c);
    __m128 vgamma = _mm_set1_ps(gamma);
    __m128 v255 = _mm_set1_ps(255.0f);
    __m128 v1_div_255 = _mm_set1_ps(1.0f / 255.0f);

    size_t restWidth = 0;
    size_t restX = 0;
        
    size_t processedWidth = width - (width % 4); // Width interpretation as group of four

    for(size_t y = 0; y < height; y++) {
        // restWidth >= 6 is a restriction to avoid memory leakage; 
        // The minimum remaining width length must be >= 5,3 (16 Byte min width length) 
        restWidth = width;
        for(size_t x = 0; x < processedWidth && restWidth >= 6; x += 4, restWidth -= 4, restX = x ) { // Processing four pixels
            size_t idx = (y * width + x) * 3;

            // Load 4 RGB pixel into SIMD register
            __m128i rgb = _mm_loadu_si128((__m128i*)(img + idx));
            const __m128i shuffle_mask = _mm_set_epi8(9,6,3,0, 11,8,5,2, 10,7,4,1, 9,6,3,0);
            __m128i r3_r2_r1_r0_b3_b2_b1_b0_g3_g2_g1_g0_r3_r2_r1_r0 = _mm_shuffle_epi8(rgb, shuffle_mask);

            // Separating R, G, and B channels and convert to float
            __m128i R = _mm_slli_si128(r3_r2_r1_r0_b3_b2_b1_b0_g3_g2_g1_g0_r3_r2_r1_r0,12);
            R = _mm_srli_si128(R, 12);
            __m128i G = _mm_srli_si128(_mm_slli_si128(r3_r2_r1_r0_b3_b2_b1_b0_g3_g2_g1_g0_r3_r2_r1_r0, 8),12); 
            __m128i B = _mm_srli_si128(_mm_slli_si128(r3_r2_r1_r0_b3_b2_b1_b0_g3_g2_g1_g0_r3_r2_r1_r0,4),12);
           
            R = _mm_cvtepu8_epi32(R);
            G = _mm_cvtepu8_epi32(G);
            B = _mm_cvtepu8_epi32(B);

            __m128 Rf = _mm_cvtepi32_ps(R);
            __m128 Gf = _mm_cvtepi32_ps(G);
            __m128 Bf = _mm_cvtepi32_ps(B);

            // Gray scale convertion
            __m128 gray = _mm_div_ps(_mm_add_ps(_mm_add_ps(_mm_mul_ps(va, Rf), _mm_mul_ps(vb, Gf)), _mm_mul_ps(vc, Bf)), vsum);

            // Applying the gammacorrection
            gray = _mm_mul_ps(gray, v1_div_255);
            __m128 log_gray = log_approx(gray);
            __m128 gamma_log_gray = _mm_mul_ps(vgamma, log_gray);
            __m128 corrected_gray = exp_approx(gamma_log_gray);
            corrected_gray = _mm_mul_ps(corrected_gray, v255);

            // Reconverting to 8-bit
            //__m128i gray8bit = _mm_cvtps_epi32(corrected_gray);
            //gray8bit = _mm_packus_epi16(gray8bit, gray8bit);
            //gray8bit = _mm_packus_epi16(gray8bit, gray8bit);
            
            __m128i corrected_gray8bit = _mm_cvtps_epi32(corrected_gray);

            // Saving the result
            //uint32_t temp;
            //_mm_store_ss((float*)&temp, _mm_castsi128_ps(gray8bit));
            //result[y * width + x] = (uint8_t)(temp & 0xFF);
            result[y * width + x] =(uint8_t) _mm_extract_epi32(corrected_gray8bit, 0);
            result[y * width + x + 1] =(uint8_t) _mm_extract_epi32(corrected_gray8bit, 1);
            result[y * width + x + 2] =(uint8_t) _mm_extract_epi32(corrected_gray8bit, 2);
            result[y * width + x + 3] =(uint8_t) _mm_extract_epi32(corrected_gray8bit, 3);
        }

        if((width % 4) == 0 && restWidth < 6){
            // Edge cases handling -> when width IS div by 4 but restWidth < 6
            for (size_t x = restX; x < width; x++) {
                size_t idx = (y * width + x) * 3;

                float R = img[idx];
                float G = img[idx + 1];
                float B = img[idx + 2];

                //  Gray scale convertion
                float gray = (a * R + b * G + c * B) / (a + b + c);

                // Gamma correction
                float gammaCorrectedValue = powf(gray / 255.0f, gamma) * 255.0f;

                result[y * width + x] = (uint8_t)gammaCorrectedValue; 
            }
        }
        else{
     // Edge cases
            for (size_t x = processedWidth; x < width; x++) {
                size_t idx = (y * width + x) * 3;

                float R = img[idx];
                float G = img[idx + 1];
                float B = img[idx + 2];

                //  Gray scale convertion
                float gray = (a * R + b * G + c * B) / (a + b + c);

                // Gamma correction
                float gammaCorrectedValue = powf(gray / 255.0f, gamma) * 255.0f;

                result[y * width + x] = (uint8_t)gammaCorrectedValue;
            }
        }
        
    }
}
