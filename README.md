# Gamma Correction – Image Processing in C

This project implements gamma correction for grayscale images in the PPM (Portable PixMap) format. Gamma correction is a nonlinear operation used to encode and decode luminance or tristimulus values in images. It's essential for adjusting the brightness of images to match the nonlinear response of display systems and the human eye.

## 📁 Project Structure

```
├── Implementation/
│   ├── Benchmark img          # Images used for benchmarking
│   ├── Makefile               # Build script
│   ├── gamma_V4.c             # Gamma correction implementation
│   ├── gamma_V4.h             # Header for gamma correction
│   ├── main.c                 # Entry point of the program
│   ├── parse.c / parse.h      # Command-line argument parsing
│   ├── read.c / read.h        # Reading PPM images
│   ├── write.c / write.h      # Writing PPM images
│   ├── input_2.ppm            # Sample input image
│   └── input_3.ppm            # Sample input image
└── README.md                  # Project overview
```

## ⚙️ How It Works

The program performs gamma correction on grayscale PPM images. The key steps are:

1. **Reading the Image**: The `read.c` module reads a PPM image file and stores its pixel data.
2. **Parsing Arguments**: The `parse.c` module handles command-line arguments, allowing users to specify the input file, output file, and gamma value.
3. **Applying Gamma Correction**: The `gamma_V4.c` module applies the gamma correction formula to each pixel:

   ```
   corrected_pixel = 255 * (original_pixel / 255) ^ gamma
   ```

   This formula adjusts the brightness of the image based on the specified gamma value.
4. **Writing the Image**: The `write.c` module writes the corrected image data back to a new PPM file.

## 🛠️ Build Instructions

To compile the program, navigate to the `Implementierung` directory and run:

```bash
make
```

This will generate an executable file, typically named `gamma_correction`.

## 🚀 Usage

After building the program, you can run it using:

```bash
./gamma_correction -i input.ppm -o output.ppm -g 2.2
```

Where:
- `-i input.ppm`: Specifies the input PPM file.
- `-o output.ppm`: Specifies the output PPM file.
- `-g 2.2`: Specifies the gamma value to apply.

## 🧪 Sample Images

The `Implementation` directory includes sample PPM images (`input_2.ppm`, `input_3.ppm`) for testing the gamma correction.


