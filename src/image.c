#include <image.h>
#include <stdint.h>
#include <stdlib.h>

GrayScaleImage *create_grayscale_image(int width, int height, int max_pixel_value)
{
    GrayScaleImage *img = (GrayScaleImage *)malloc(sizeof(GrayScaleImage));
    if (!img)
        return NULL;

    img->height = height;
    img->width = width;
    img->max_pixel_value = max_pixel_value;
    img->pixels = (int *)malloc(width * height * sizeof(int32_t));
    if (!img->pixels)
    {
        free(img);
        return NULL;
    }

    return img;
}

void destroy_grayscale_image(GrayScaleImage *img)
{
    if (img)
    {
        img->width = img->height = img->max_pixel_value = 0;
        free(img->pixels);
        img->pixels = NULL;
		free(img);
    }
}

GrayScaleImage *scale_grayscale_image_average(const GrayScaleImage *restrict src, const int target_width)
{
    // Upscaling is not accepted.
    if (target_width >= src->width)
    {
        return NULL;
    }

    const int target_height = (src->height * target_width) / (src->width) / WIDTH_HEIGH_SCALE_FACTOR;
    const int patch_width = src->width / target_width;
    const int patch_height = src->height / target_height;

    GrayScaleImage *scaled = create_grayscale_image(target_width, target_height, src->max_pixel_value);

    for (int i = 0; i < target_height; i++)
    {
        for (int j = 0; j < target_width; j++)
        {
            IMG_INDEX(scaled->pixels, target_width, i, j) = 0;

            for (int ii = 0; ii < patch_height; ii++)
            {
                for (int jj = 0; jj < patch_width; jj++)
                {
                    IMG_INDEX(scaled->pixels, target_width, i, j) +=
                        IMG_INDEX(src->pixels, src->width, i * patch_height + ii, j * patch_width + jj);
                }
            }

            IMG_INDEX(scaled->pixels, target_width, i, j) /= patch_width * patch_height;
        }
    }

    return scaled;
}
