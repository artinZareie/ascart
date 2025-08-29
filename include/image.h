#pragma once

#include <stddef.h>
#include <stdint.h>

#define WIDTH_HEIGH_SCALE_FACTOR (2)
#define IMG_INDEX(ARR, WIDTH, I, J) ((ARR)[(I) * (WIDTH) + (J)])

typedef uint8_t (*RGB2GrayFunc)(uint8_t, uint8_t, uint8_t);
typedef uint8_t (*GrayAlphaFunc)(uint8_t, uint8_t, uint8_t);

typedef struct
{
    size_t width, height;
    uint8_t max_pixel_value;
    uint8_t *pixels;
} GrayScaleImage8bit;

GrayScaleImage8bit *create_grayscale_image_8bit(size_t width, size_t height, uint8_t max_pixel_value);
void destroy_grayscale_image_8_bit(GrayScaleImage8bit *image);

GrayScaleImage8bit *scale_grayscale_image_average_8bit(const GrayScaleImage8bit *restrict src, const size_t target_width);

GrayScaleImage8bit *load_image_as_grayscale_stb_8bit(const char *filename, RGB2GrayFunc rgb2gray, GrayAlphaFunc gray_alpha);

uint8_t rgb2gray_luminosity_8bit(uint8_t r, uint8_t g, uint8_t b);
uint8_t rgb2gray_average_8bit(uint8_t r, uint8_t g, uint8_t b);

uint8_t apply_gray_alpha_8bit(uint8_t g, uint8_t a, uint8_t max_pixel);
