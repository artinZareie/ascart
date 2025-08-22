#pragma once

#include "image.h"
#include <stddef.h>

typedef enum
{
    SAVE_SUCCESS = 0,
    SAVE_FOPEN_ERR,
} FileSaveStat;

typedef struct
{
    size_t width, height;
    char *pixels;
} CharacterASCIIImage;

CharacterASCIIImage *create_character_ascii_image(size_t width, size_t height);
void destroy_character_ascii_image(CharacterASCIIImage *image);

CharacterASCIIImage *generate_brightness_ascii_art(const GrayScaleImage *restrict src);
FileSaveStat save_character_ascii_image_to_file(const CharacterASCIIImage *restrict src, const char *dest);
