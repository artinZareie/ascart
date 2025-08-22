#include <helpers.h>
#include <pgm.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

GrayScaleImage *read_pgm_p2(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        errorf("PGM File Not Found!");
        return 0;
    }

    char buff[AGP_PGM_BUFF_SIZE];
    fscanf(file, "%s", buff);

    if (strcmp(buff, "P2") != 0)
    {
        fclose(file);
        errorf("Target File Invalid Format!");
        return 0;
    }

    size_t width, height, max_pixel_value;
    if (fscanf(file, "%zu %zu", &width, &height) != 2)
    {
        fclose(file);
        errorf("Target PGM file corrupted.");
        return 0;
    }

    if (fscanf(file, "%zu", &max_pixel_value) != 1)
    {
        fclose(file);
        errorf("Target PGM file corrupted.");
        return 0;
    }

    GrayScaleImage *image = create_grayscale_image(width, height, max_pixel_value);

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            size_t pixel_value;

            if (fscanf(file, "%zu", &pixel_value) != 1)
            {
                errorf("Dimension Mismatch!");
            }

			elogf("%d %d | %zu %zu", i, j, width, height);

            IMG_INDEX(image->pixels, width, i, j) = pixel_value;
        }
    }

    fclose(file);

    return image;
}
