#include <ascii_engine.h>
#include <helpers.h>
#include <image.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int return_code = 1;
    GrayScaleImage8bit *original_image = NULL;
    GrayScaleImage8bit *scaled_image = NULL;
    CharacterASCIIImage *ascii_art = NULL;
    GrayScaleImage8bit *image_to_process = NULL;

    if (argc != 3 && argc != 4)
    {
        errorf("Usage: %s <input.pgm> <output.txt> [target_width]", argv[0]);
        return 1;
    }

    original_image = load_image_as_grayscale_stb_8bit(argv[1], rgb2gray_average_8bit, apply_gray_alpha_8bit);
    if (!original_image)
    {
        errorf("Failed to read image file: %s", argv[1]);
        goto cleanup;
    }

    const size_t target_width = (argc == 4) ? (size_t)atoi(argv[3]) : original_image->width;

    if (target_width > original_image->width)
    {
        errorf("Target width (%zu) must be <= original width (%zu)", target_width, original_image->width);
        goto cleanup;
    }

    if (target_width < original_image->width)
    {
        scaled_image = scale_grayscale_image_average_8bit(original_image, target_width);
        if (!scaled_image)
        {
            errorf("Failed to scale image");
            goto cleanup;
        }
        image_to_process = scaled_image;
    }
    else
    {
        image_to_process = original_image;
    }

    ascii_art = generate_brightness_ascii_art(image_to_process);
    if (!ascii_art)
    {
        errorf("Failed to generate ASCII art");
        goto cleanup;
    }

    if (save_character_ascii_image_to_file(ascii_art, argv[2]) != SAVE_SUCCESS)
    {
        errorf("Failed to save ASCII art to: %s", argv[2]);
        goto cleanup;
    }

    elogf("Successfully converted %s to ASCII art in %s", argv[1], argv[2]);
    return_code = 0;

cleanup:
    destroy_grayscale_image_8_bit(original_image);
    destroy_grayscale_image_8_bit(scaled_image);
    destroy_character_ascii_image(ascii_art);

    return return_code;
}
