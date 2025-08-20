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
    if (argc != 4 && argc != 3)
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

    const int target_width = argc == 4 ? atoi(argv[3]) : image_width;

    // // TODO: relax this constraint in future.
    // if (image_width % target_width != 0)
    // {
    //     perror("Incorrect dimension");
    //     fclose(target_file);
    //     fclose(image_file);
    //     return 1;
    // }

    const int target_height = (image_height * target_width) / image_width / 2;

    const int patch_width = image_width / target_width;
    const int patch_height = image_height / target_height;
    const int patch_size = patch_height * patch_width;

    int *real_image_grid = (int *)malloc(image_width * image_height * sizeof(int));
    int *scaled_image_grid = (int *)malloc(target_width * target_height * sizeof(int));

    int image_darkest = 255;

    if (fscanf(image_file, "%d", &image_darkest) != 1 || image_darkest <= 0)
    {
        perror("corrupted input");
        fclose(target_file);
        fclose(image_file);
        return 1;
    }

    int col = 0;
    int row = 0;

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

            real_image_grid[row * image_width + col] = pixel;
            col++;
            ptr = tptr;

            if (col == image_width)
            {
                col = 0;
                row++;
            }
        }
    }

    for (int i = 0; i < target_height; i++)
    {
        for (int j = 0; j < target_width; j++)
        {
            int sum = 0;

            for (int ii = 0; ii < patch_height; ii++)
            {
                for (int jj = 0; jj < patch_width; jj++)
                {
                    // sum += real_image_grid[i * patch_height + ii][j * patch_width + j]
                    sum += real_image_grid[(i * patch_height + ii) * image_width + j * patch_width + jj];
                }
            }

            sum /= patch_size;
            scaled_image_grid[i * target_width + j] = sum;
        }
    }

    for (int i = 0; i < target_height; i++)
    {
        for (int j = 0; j < target_width; j++)
        {
            int pixel = scaled_image_grid[i * target_width + j];
            int idx = (pixel * (MAX_CHARS - 1)) / image_darkest;
            fputc(ASCII_CHARACTER[idx], target_file);
        }

        fputc('\n', target_file);
    }

    free(scaled_image_grid);
    free(real_image_grid);
    fclose(target_file);
    fclose(image_file);

    return 0;
}
