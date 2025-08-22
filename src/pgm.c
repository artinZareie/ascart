#include <helpers.h>
#include <pgm.h>
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

    if (strcmp(buff, "P2\n") != 0)
    {
        fclose(file);
        errorf("Target File Invalid Format!");
        return 0;
    }

    int width, height, max_pixel_value;
    if (fscanf(file, "%d %d", &width, &height) != 2)
    {
        fclose(file);
        errorf("Target PGM file corrupted.");
        return 0;
    }

    if (fscanf(file, "%d", &max_pixel_value) != 1)
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
            int pixel_value;

            if (fscanf(file, "%d", &pixel_value) != 1)
            {
                errorf("Dimension Mismatch!");
            }

            IMG_INDEX(image->pixels, width, i, j) = pixel_value;
        }
    }

    fclose(file);

    return image;
}
