#pragma once

#include <stddef.h>
#include <stdint.h>

#define WIDTH_HEIGH_SCALE_FACTOR (2)
#define IMG_INDEX(ARR, WIDTH, I, J) ((ARR)[(I) * (WIDTH) + (J)])

typedef size_t (*RGB2GrayFunc)(size_t, size_t, size_t);
typedef size_t (*GrayAlphaFunc)(size_t, size_t, size_t);

typedef struct
{
    size_t width, height, max_pixel_value;
    size_t *pixels;
} GrayScaleImage;

GrayScaleImage *create_grayscale_image(size_t width, size_t height, size_t max_pixel_value);
void destroy_grayscale_image(GrayScaleImage *image);

GrayScaleImage *scale_grayscale_image_average(const GrayScaleImage *restrict src, const size_t target_width);

GrayScaleImage *load_image_as_grayscale_stb(const char *filename, RGB2GrayFunc rgb2gray, GrayAlphaFunc gray_alpha);

size_t rgb2gray_luminosity(size_t r, size_t g, size_t b);
size_t rgb2gray_average(size_t r, size_t g, size_t b);

size_t apply_gray_alpha(size_t g, size_t a, size_t max_pixel);
