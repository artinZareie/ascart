#pragma once

#include <stdint.h>

#define WIDTH_HEIGH_SCALE_FACTOR (2)
#define IMG_INDEX(ARR, WIDTH, I, J) ((ARR)[(I) * (WIDTH) + (J)])

typedef struct
{
    int width, height, max_pixel_value;
    int *pixels;
} GrayScaleImage;

GrayScaleImage *create_grayscale_image(int width, int height, int max_pixel_value);
void destroy_grayscale_image(GrayScaleImage *image);

GrayScaleImage *scale_grayscale_image_average(const GrayScaleImage *restrict src, const int target_width);
