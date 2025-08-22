#include <ascii_engine.h>
#include <helpers.h>
#include <image.h>
#include <pgm.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if (argc != 4 && argc != 3)
    {
        errorf("Usage: %s <input.pgm> <output.txt> [target_width]", argv[0]);
        return 1;
    }

    GrayScaleImage *original_image = read_pgm_p2(argv[1]);
    if (!original_image)
    {
        errorf("Failed to read PGM file: %s", argv[1]);
        return 1;
    }

    debugf("Original image: %zux%zu, max value: %zu", original_image->width, original_image->height,
           original_image->max_pixel_value);

    const size_t target_width = (argc == 4) ? (size_t)atoi(argv[3]) : original_image->width;

    if (target_width >= original_image->width)
    {
        errorf("Target width (%zu) must be less than original width (%zu)", target_width, original_image->width);
        destroy_grayscale_image(original_image);
        return 1;
    }

    GrayScaleImage *scaled_image = scale_grayscale_image_average(original_image, target_width);
    if (!scaled_image)
    {
        errorf("Failed to scale image");
        destroy_grayscale_image(original_image);
        return 1;
    }

    debugf("Scaled image: %zux%zu", scaled_image->width, scaled_image->height);

    CharacterASCIIImage *ascii_art = generate_brightness_ascii_art(scaled_image);
    if (!ascii_art)
    {
        errorf("Failed to generate ASCII art");
        destroy_grayscale_image(original_image);
        destroy_grayscale_image(scaled_image);
        return 1;
    }

    FileSaveStat save_status = save_character_ascii_image_to_file(ascii_art, argv[2]);
    if (save_status != SAVE_SUCCESS)
    {
        errorf("Failed to save ASCII art to: %s", argv[2]);
        destroy_grayscale_image(original_image);
        destroy_grayscale_image(scaled_image);
        destroy_character_ascii_image(ascii_art);
        return 1;
    }

    elogf("Successfully converted %s to ASCII art in %s", argv[1], argv[2]);
    debugf("Output size: %zux%zu characters", ascii_art->width, ascii_art->height);

    destroy_grayscale_image(original_image);
    destroy_grayscale_image(scaled_image);
    destroy_character_ascii_image(ascii_art);

    return 0;
}
