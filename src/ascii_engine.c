#include <ascii_engine.h>
#include <helpers.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

static const char *const brightness_ascii_characters =
    " `.-':_,^=;><+!rc*/z?sLTv)J7(|Fi{C}fI31tlu[neoZ5Yxjya]2ESwqkP6h9d4VpOGbUAKXHm8RD#$Bg0MNWQ%&@";
static const size_t brightness_ascii_characters_count = 92;

CharacterASCIIImage *create_character_ascii_image(size_t width, size_t height)
{
    CharacterASCIIImage *img = (CharacterASCIIImage *)malloc(sizeof(CharacterASCIIImage));

    if (!img)
    {
        return 0;
    }

    img->width = width;
    img->height = height;
    img->pixels = (char *)malloc(width * height * sizeof(char));

    if (!img->pixels)
    {
        free(img);
        return 0;
    }

    return img;
}

void destroy_character_ascii_image(CharacterASCIIImage *image)
{
    if (image)
    {
        image->height = image->width = 0;

        if (!image->pixels)
            free(image->pixels);
        image->pixels = NULL;
    }
}

CharacterASCIIImage *generate_brightness_ascii_art(const GrayScaleImage8bit *restrict src)
{
    CharacterASCIIImage *ascart = create_character_ascii_image(src->width, src->height);

#pragma omp parallel for
    for (int i = 0; i < src->height; i++)
    {
        for (int j = 0; j < src->width; j++)
        {
            size_t pixel = IMG_INDEX(src->pixels, src->width, i, j);
            char ascii = brightness_ascii_characters[((int)pixel * ((int)brightness_ascii_characters_count - 1)) /
                                                     (int)src->max_pixel_value]; // NOTE: Conversion to int is crucial
                                                                                 // due to overflow.
            IMG_INDEX(ascart->pixels, ascart->width, i, j) = ascii;
        }
    }

    return ascart;
}

FileSaveStat save_character_ascii_image_to_file(const CharacterASCIIImage *restrict src, const char *dest)
{
    FILE *file = fopen(dest, "w");

    if (!file)
    {
        return SAVE_FOPEN_ERR;
    }

    for (int i = 0; i < src->height; i++)
    {
        for (int j = 0; j < src->width; j++)
        {
            fputc(IMG_INDEX(src->pixels, src->width, i, j), file);
        }

        fputc('\n', file);
    }

    fclose(file);
    return SAVE_SUCCESS;
}
