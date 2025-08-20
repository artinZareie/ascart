#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUFF 1024

const char *const ASCII_CHARACTER =
    " `.-':_,^=;><+!rc*/z?sLTv)J7(|Fi{C}fI31tlu[neoZ5Yxjya]2ESwqkP6h9d4VpOGbUAKXHm8RD#$Bg0MNWQ%&@";
const int MAX_CHARS = 92;

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        perror("insufficient/too many arguments.");
        return 1;
    }

    FILE *image_file = fopen(argv[1], "r");

    if (image_file == NULL)
    {
        perror("fopen() failed");
        return 1;
    }

    FILE *target_file = fopen(argv[2], "w");

    if (target_file == NULL)
    {
        perror("fopen() failed");
        fclose(image_file);
        return 1;
    }

    int image_width, image_height;
    char image_type[20];
    fgets(image_type, 19, image_file);
    if (strcmp("P2\n", image_type) != 0)
    {
        perror("format error: P2 format required");
        fclose(target_file);
        fclose(image_file);
        return 1;
    }

    if (fscanf(image_file, "%d %d", &image_width, &image_height) != 2)
    {
        perror("corrupted input");
        fclose(target_file);
        fclose(image_file);
        return 1;
    }

    int image_darkest = 255;

    if (fscanf(image_file, "%d", &image_darkest) != 1 || image_darkest <= 0)
    {
        perror("corrupted input");
        fclose(target_file);
        fclose(image_file);
        return 1;
    }

    int this_line_characters = 0;

    getc(image_file); // Ignore the newline character.
    char line[MAX_BUFF];
    while (fgets(line, MAX_BUFF, image_file)) // Read line by line.
    {
        char *ptr = line, *tptr;

        while (*ptr != '\0')
        {
            if (*ptr == '\n')
            {
                break;
            }

            while (isspace((unsigned char)*ptr))
            {
                ptr++;
            }

            int pixel = strtol(ptr, &tptr, 10);

            if (ptr == tptr)
            {
                ptr++;
                continue;
            }

            int ascii_index = (pixel * (MAX_CHARS - 1)) / image_darkest;
            fputc(ASCII_CHARACTER[ascii_index], target_file);
            this_line_characters++;
            ptr = tptr;

            if (this_line_characters == image_width)
            {
                fputc('\n', target_file);
                this_line_characters = 0;
            }
        }
    }

    fclose(target_file);
    fclose(image_file);

    return 0;
}
