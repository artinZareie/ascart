#pragma once

#include <stddef.h>
#include <stdint.h>

#define WIDTH_HEIGH_SCALE_FACTOR (2)
#define IMG_INDEX(ARR, WIDTH, I, J) ((ARR)[(I) * (WIDTH) + (J)])

typedef struct
{
    size_t width, height, max_pixel_value;
    size_t *pixels;
} GrayScaleImage;

GrayScaleImage *create_grayscale_image(size_t width, size_t height, size_t max_pixel_value);
void destroy_grayscale_image(GrayScaleImage *image);

GrayScaleImage *scale_grayscale_image_average(const GrayScaleImage *restrict src, const size_t target_width);
