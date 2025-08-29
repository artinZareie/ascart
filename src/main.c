#include <ascii_engine.h>
#include <helpers.h>
#include <image.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);

    int return_code = 1;
    GrayScaleImage8bit *original_image = NULL;
    GrayScaleImage8bit *scaled_image = NULL;
    CharacterASCIIImage *ascii_art = NULL;
    GrayScaleImage8bit *image_to_process = NULL;

    if (argc != 2)
    {
        endwin();
        errorf("Usage: %s <input_image>", argv[0]);
        return 1;
    }

    int term_rows, term_cols;
    getmaxyx(stdscr, term_rows, term_cols);

    original_image = load_image_as_grayscale_stb_8bit(argv[1], rgb2gray_average_8bit, apply_gray_alpha_8bit);
    if (!original_image)
    {
        goto cleanup;
    }

    size_t target_w_from_cols = term_cols;
    size_t target_w_from_rows = (term_rows * original_image->width * WIDTH_HEIGH_SCALE_FACTOR) / original_image->height;

    size_t target_width = (target_w_from_cols < target_w_from_rows) ? target_w_from_cols : target_w_from_rows;

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

    clear();
    int start_y = (term_rows - ascii_art->height) / 2;
    int start_x = (term_cols - ascii_art->width) / 2;

    for (size_t i = 0; i < ascii_art->height; ++i)
    {
        for (size_t j = 0; j < ascii_art->width; ++j)
        {
            mvaddch(start_y + i, start_x + j, IMG_INDEX(ascii_art->pixels, ascii_art->width, i, j));
        }
    }

    refresh();
    getch();

    return_code = 0;

cleanup:
    destroy_grayscale_image_8_bit(original_image);
    destroy_grayscale_image_8_bit(scaled_image);
    destroy_character_ascii_image(ascii_art);
    endwin();

    return return_code;
}
