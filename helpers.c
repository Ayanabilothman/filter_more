#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    int pixel;
    for (int r = 0; r < height; r++)
    {
        for (int c = 0; c < width; c++)
        {
            pixel = round((image[r][c].rgbtBlue + image[r][c].rgbtGreen + image[r][c].rgbtRed) / 3.0);
            image[r][c].rgbtBlue = pixel;
            image[r][c].rgbtGreen = pixel;
            image[r][c].rgbtRed = pixel;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    int c = width - 1;
    int W = round(width / 2);
    int widthcopy = width - W;
    RGBTRIPLE copy[height][widthcopy];

    for (int r = 0; r < height; r++)
    {
        for (int half_w = 0; half_w < widthcopy; half_w++)
        {
            copy[r][half_w] = image[r][half_w];
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (j < W)
            {
                image[i][j] = image[i][c];
            }
            else
            {
                image[i][j] = copy[i][c];
            }
            c--;
        }
        c = width - 1;
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE copy[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }

    // 3 values for Blue , Red, Green
    int value[3];
    int values_sum[3] = {0, 0, 0};
    float pixels_count = 0.0;
    for (int r = 0; r < height; r++)
    {
        for (int c = 0; c < width; c++)
        {
            // rr and cc are the positions of kernel
            for (int rr = r - 1; rr < r + 2; rr++)
            {
                for (int cc = c - 1; cc < c + 2; cc++)
                {
                    // condition to sure that it isn't outside the image
                    if (rr < 0 || rr > height - 1 || cc < 0 || cc > width - 1)
                    {
                        value[0] = 0;
                        value[1] = 0;
                        value[2] = 0;
                    }
                    else
                    {
                        value[0] = copy[rr][cc].rgbtBlue;
                        value[1] = copy[rr][cc].rgbtGreen;
                        value[2] = copy[rr][cc].rgbtRed;
                        pixels_count++;
                    }
                    values_sum[0] += value[0];
                    values_sum[1] += value[1];
                    values_sum[2] += value[2];
                }
            }
            image[r][c].rgbtBlue =  round(values_sum[0] / pixels_count);
            image[r][c].rgbtGreen = round(values_sum[1] / pixels_count);
            image[r][c].rgbtRed = round(values_sum[2] / pixels_count);
            pixels_count = 0.0;
            values_sum[0] = 0;
            values_sum[1] = 0;
            values_sum[2] = 0;
        }
    }
    return;
}

// to ensure that the value does not exceed 255
int max(double max)
{
    if (max > 255)
    {
        max = 255;
    }
    return max;
}


// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE copy[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }

    long value_BX, value_GX, value_RX, value_BY, value_GY, value_RY;
    long Sum_BX = 0, Sum_GX = 0, Sum_RX = 0, Sum_BY = 0, Sum_GY = 0, Sum_RY = 0;
    int GX[] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
    int GY[] = {-1, -2, -1, 0, 0, 0, 1, 2, 1};
    int i = 0;
    for (int r = 0; r < height; r++)
    {
        for (int c = 0; c < width; c++)
        {
            // rr and cc are the positions of kernel
            for (int rr = r - 1; rr < r + 2; rr++)
            {
                for (int cc = c - 1; cc < c + 2; cc++)
                {
                    // condition to sure that it isn't outside the image
                    if (rr < 0 || rr > height - 1 || cc < 0 || cc > width - 1)
                    {
                        value_BX = 0;
                        value_GX = 0;
                        value_RX = 0;
                        value_BY = 0;
                        value_GY = 0;
                        value_RY = 0;
                    }
                    else
                    {
                        value_BX = copy[rr][cc].rgbtBlue * GX[i];
                        value_GX = copy[rr][cc].rgbtGreen * GX[i];
                        value_RX = copy[rr][cc].rgbtRed * GX[i];
                        value_BY = copy[rr][cc].rgbtBlue * GY[i];
                        value_GY = copy[rr][cc].rgbtGreen * GY[i];
                        value_RY = copy[rr][cc].rgbtRed * GY[i];
                    }
                    i++;
                    Sum_BX += value_BX;
                    Sum_GX += value_GX;
                    Sum_RX += value_RX;
                    Sum_BY += value_BY;
                    Sum_GY += value_GY;
                    Sum_RY += value_RY;
                }
            }
            //pow & sqrt  take a double value as an argument and output double value
            image[r][c].rgbtBlue = max(round(sqrt(pow(Sum_BX, 2) + pow(Sum_BY, 2))));
            image[r][c].rgbtGreen = max(round(sqrt(pow(Sum_GX, 2) + pow(Sum_GY, 2))));
            image[r][c].rgbtRed = max(round(sqrt(pow(Sum_RX, 2) + pow(Sum_RY, 2))));
            i = 0;
            Sum_BX = 0;
            Sum_RX = 0;
            Sum_GX = 0;
            Sum_BY = 0;
            Sum_GY = 0;
            Sum_RY = 0;
        }
    }
    return;
}
