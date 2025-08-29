#include <helpers.h>
#include <image.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

GrayScaleImage8bit *create_grayscale_image_8bit(size_t width, size_t height, uint8_t max_pixel_value)
{
    GrayScaleImage8bit *img = (GrayScaleImage8bit *)malloc(sizeof(GrayScaleImage8bit));
    if (!img)
        return NULL;

    img->height = height;
    img->width = width;
    img->max_pixel_value = max_pixel_value;
    img->pixels = (uint8_t *)malloc(width * height * sizeof(uint8_t));
    if (!img->pixels)
    {
        free(img);
        return NULL;
    }

    return img;
}

void destroy_grayscale_image_8_bit(GrayScaleImage8bit *img)
{
    if (img)
    {
        img->width = img->height = img->max_pixel_value = 0;

        if (img->pixels)
            free(img->pixels);

        img->pixels = NULL;
        free(img);
    }
}

GrayScaleImage8bit *scale_grayscale_image_average_8bit(const GrayScaleImage8bit *restrict src,
                                                       const size_t target_width)
{
    // Upscaling is not accepted.
    if (target_width >= src->width)
    {
        return NULL;
    }

    const size_t target_height = (src->height * target_width) / (src->width) / WIDTH_HEIGH_SCALE_FACTOR;
    const size_t patch_width = src->width / target_width;
    const size_t patch_height = src->height / target_height;

    GrayScaleImage8bit *scaled = create_grayscale_image_8bit(target_width, target_height, src->max_pixel_value);

#pragma omp parallel for
    for (size_t i = 0; i < target_height; i++)
    {
        for (size_t j = 0; j < target_width; j++)
        {
            // IMG_INDEX(scaled->pixels, target_width, i, j) = 0;
            uint32_t sum = 0;

            for (size_t ii = 0; ii < patch_height; ii++)
            {
                for (size_t jj = 0; jj < patch_width; jj++)
                {
                    sum += IMG_INDEX(src->pixels, src->width, i * patch_height + ii, j * patch_width + jj);
                }
            }

            IMG_INDEX(scaled->pixels, target_width, i, j) =
                sum / (patch_width * patch_height); // NOTE: Conversion to int is crucial due to overflow.
        }
    }

    return scaled;
}

GrayScaleImage8bit *load_image_as_grayscale_stb_8bit(const char *filename, RGB2GrayFunc rgb2gray,
                                                     GrayAlphaFunc gray_alpha)
{
    int width, height, n_channels;

    unsigned char *img_data = stbi_load(filename, &width, &height, &n_channels, false);
    if (img_data == NULL || width <= 0 || height <= 0 || n_channels <= 0)
    {
        errorf("stbi_load() failed.");
        goto failure_exit;
    }

    GrayScaleImage8bit *gscale_image = create_grayscale_image_8bit(width, height, 255);
    if (!gscale_image)
    {
        errorf("create_grayscale_image() failed.");
        goto failure_exit;
    }

    for (size_t i = 0; i < height; i++)
    {
        for (size_t j = 0; j < width; j++)
        {
            size_t pixel_gray = 0;
            unsigned char *pixel_offset = img_data + (i * width + j) * n_channels;

            switch (n_channels)
            {
            case 1: // Grayscale case
                pixel_gray = pixel_offset[0];
                break;

            case 2: // Grayscale + Alpha Channel
                pixel_gray = gray_alpha(pixel_offset[0], pixel_offset[1], 255);
                break;

            case 3: // RGB
                pixel_gray = rgb2gray(pixel_offset[0], pixel_offset[1], pixel_offset[2]);
                break;

            case 4: // RGB + Alpha Channel
                pixel_gray =
                    gray_alpha(rgb2gray(pixel_offset[0], pixel_offset[1], pixel_offset[2]), pixel_offset[3], 255);
                break;

            default:

                errorf("stbi_image() failed.");
                goto failure_exit;
            }

            IMG_INDEX(gscale_image->pixels, width, i, j) = pixel_gray;
        }
    }

    stbi_image_free(img_data);
    return gscale_image;

failure_exit:
    if (!img_data)
        stbi_image_free(img_data);

    if (!gscale_image)
        destroy_grayscale_image_8_bit(gscale_image);

    return NULL;
}

uint8_t rgb2gray_luminosity_8bit(uint8_t r, uint8_t g, uint8_t b)
{
    return (uint8_t)round(0.299 * r + 0.587 * g + 0.114 * b);
}

uint8_t rgb2gray_average_8bit(uint8_t r, uint8_t g, uint8_t b)
{
    return ((int)r + g + b) / 3; // NOTE: Conversion to int is crucial due to overflow.
}

uint8_t apply_gray_alpha_8bit(uint8_t g, uint8_t a, uint8_t max_pixel)
{
    return ((int)g * a) / max_pixel; // NOTE: Conversion to int is crucial due to overflow.
}
