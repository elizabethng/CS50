#include "helpers.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// GENERAL
// input is an array of RGBTRIPLE struct, which has rgbtBlue, rgbtGreen, and rgbtRed

// --------------------------
// Convert image to grayscale
// --------------------------
// grayscale sets pixel's rgb values to the average of r, g, and b
// loop through each i and j dimension and average the three values
// (I will need to parse out the r/g/b values)
// and then return them to the same spot
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // calculate the average of the colors
            float avg = (image[i][j].rgbtRed + image[i][j].rgbtGreen + image[i][j].rgbtBlue) / 3.0;
            // reasign the values - but this is inside a function call! Like swap so I need to follow
            // that approach. huh this worked!
            image[i][j].rgbtRed = round(avg);
            image[i][j].rgbtGreen = round(avg);
            image[i][j].rgbtBlue = round(avg);
        }
    }
    return;
}

// --------------------------
// Convert image to sepia
// --------------------------
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // calculate new color values
            float sepiaRed = .393 * image[i][j].rgbtRed + .769 * image[i][j].rgbtGreen + .189 * image[i][j].rgbtBlue;
            float sepiaGreen = .349 * image[i][j].rgbtRed + .686 * image[i][j].rgbtGreen + .168 * image[i][j].rgbtBlue;
            float sepiaBlue = .272 * image[i][j].rgbtRed + .534 * image[i][j].rgbtGreen + .131 * image[i][j].rgbtBlue;

            // constraints
            if (sepiaRed > 255)
                sepiaRed = 255;
            if (sepiaGreen > 255)
                sepiaGreen = 255;
            if (sepiaBlue > 255)
                sepiaBlue = 255;

            // update values
            image[i][j].rgbtRed = round(sepiaRed);
            image[i][j].rgbtGreen = round(sepiaGreen);
            image[i][j].rgbtBlue = round(sepiaBlue);
        }
    }

    return;
}

// --------------------------
// Reflect image horizontally
// --------------------------
// simple case of
// 123 --> 321
// 456 --> 654
// 789 --> 987
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // do for 1
    // want to start and the end
    // tmp[width] = image[0][0];
    // tmp[width - 1] = image[0][1];
    // tmp[width - 2] = image[0][2];

    // so I need to loop over everything to create the tmp array
    // then loop back over to do the assignment, since
    // image[i] = tmp gave "not assignable" error

    // loop through the rows (order unchanged)
    for (int i = 0; i < height; i++)
    {
        // Initalize temporary row vector of array
        // do I need to be careful of garbage here?
        RGBTRIPLE tmp[width];

        // loop through the columns and reverse
        for (int j = 0; j < width; j++)
        {
            tmp[width - j - 1] = image[i][j];
        }

        // re-assign the reversed output
        for (int j = 0; j < width; j++)
        {
            image[i][j] = tmp[j];
        }
    }
    return;
}

// --------------------------
// Blur image
// --------------------------
// since we are working in both directions, we now need a place-
// holder array that is the same size as image
// will also need some if statements for handling edge cases
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE tmp[height][width];

    // First loop through all cells to fill out tmp
    // loop through the rows
    for (int i = 0; i < height; i++)
    {
        // loop through the columns
        for (int j = 0; j < width; j++)
        {
            float sumRed;
            float sumGreen;
            float sumBlue;
            float N;

            if (i == 0 & j == 0)
            {
                // top left corner
                sumRed = image[i][j].rgbtRed + image[i][j + 1].rgbtRed + image[i + 1][j].rgbtRed + image[i + 1][j + 1].rgbtRed;
                sumGreen =
                    image[i][j].rgbtGreen + image[i][j + 1].rgbtGreen + image[i + 1][j].rgbtGreen + image[i + 1][j + 1].rgbtGreen;
                sumBlue = image[i][j].rgbtBlue + image[i][j + 1].rgbtBlue + image[i + 1][j].rgbtBlue + image[i + 1][j + 1].rgbtBlue;
                N = 4.0;
            }
            else if (i == 0 & j == (width - 1))
            {
                // top right corner (ck)
                sumRed = image[i][j].rgbtRed + image[i][j - 1].rgbtRed + image[i + 1][j - 1].rgbtRed + image[i + 1][j].rgbtRed;
                sumGreen =
                    image[i][j].rgbtGreen + image[i][j - 1].rgbtGreen + image[i + 1][j - 1].rgbtGreen + image[i + 1][j].rgbtGreen;
                sumBlue = image[i][j].rgbtBlue + image[i][j - 1].rgbtBlue + image[i + 1][j - 1].rgbtBlue + image[i + 1][j].rgbtBlue;
                N = 4.0;
            }
            else if (i == (height - 1) & j == 0)
            {
                // bottom left corner (ck)
                sumRed = image[i][j].rgbtRed + image[i - 1][j].rgbtRed + image[i - 1][j + 1].rgbtRed + image[i][j + 1].rgbtRed;
                sumGreen =
                    image[i][j].rgbtGreen + image[i - 1][j].rgbtGreen + image[i - 1][j + 1].rgbtGreen + image[i][j + 1].rgbtGreen;
                sumBlue = image[i][j].rgbtBlue + image[i - 1][j].rgbtBlue + image[i - 1][j + 1].rgbtBlue + image[i][j + 1].rgbtBlue;
                N = 4.0;
            }
            else if (i == (height - 1) & j == (width - 1))
            {
                // bottom right corner (ck)
                sumRed = image[i][j].rgbtRed + image[i - 1][j - 1].rgbtRed + image[i - 1][j].rgbtRed + image[i][j - 1].rgbtRed;
                sumGreen =
                    image[i][j].rgbtGreen + image[i - 1][j - 1].rgbtGreen + image[i - 1][j].rgbtGreen + image[i][j - 1].rgbtGreen;
                sumBlue = image[i][j].rgbtBlue + image[i - 1][j - 1].rgbtBlue + image[i - 1][j].rgbtBlue + image[i][j - 1].rgbtBlue;
                N = 4.0;
            }
            else if (i == 0)
            {
                // top row (no i - 1)
                sumRed = image[i][j - 1].rgbtRed + image[i][j].rgbtRed + image[i][j + 1].rgbtRed + image[i + 1][j - 1].rgbtRed +
                         image[i + 1][j].rgbtRed + image[i + 1][j + 1].rgbtRed;
                sumGreen = image[i][j - 1].rgbtGreen + image[i][j].rgbtGreen + image[i][j + 1].rgbtGreen +
                           image[i + 1][j - 1].rgbtGreen + image[i + 1][j].rgbtGreen + image[i + 1][j + 1].rgbtGreen;
                sumBlue = image[i][j - 1].rgbtBlue + image[i][j].rgbtBlue + image[i][j + 1].rgbtBlue +
                          image[i + 1][j - 1].rgbtBlue + image[i + 1][j].rgbtBlue + image[i + 1][j + 1].rgbtBlue;
                N = 6.0;
            }
            else if (i == height - 1)
            {
                // bottom row (no i + 1)
                sumRed = image[i - 1][j - 1].rgbtRed + image[i - 1][j].rgbtRed + image[i - 1][j + 1].rgbtRed +
                         image[i][j - 1].rgbtRed + image[i][j].rgbtRed + image[i][j + 1].rgbtRed;
                sumGreen = image[i - 1][j - 1].rgbtGreen + image[i - 1][j].rgbtGreen + image[i - 1][j + 1].rgbtGreen +
                           image[i][j - 1].rgbtGreen + image[i][j].rgbtGreen + image[i][j + 1].rgbtGreen;
                sumBlue = image[i - 1][j - 1].rgbtBlue + image[i - 1][j].rgbtBlue + image[i - 1][j + 1].rgbtBlue +
                          image[i][j - 1].rgbtBlue + image[i][j].rgbtBlue + image[i][j + 1].rgbtBlue;
                N = 6.0;
            }
            else if (j == 0)
            {
                // left edge (no j - 1)
                sumRed = image[i - 1][j].rgbtRed + image[i - 1][j + 1].rgbtRed + image[i][j].rgbtRed + image[i][j + 1].rgbtRed +
                         image[i + 1][j].rgbtRed + image[i + 1][j + 1].rgbtRed;
                sumGreen = image[i - 1][j].rgbtGreen + image[i - 1][j + 1].rgbtGreen + image[i][j].rgbtGreen +
                           image[i][j + 1].rgbtGreen + image[i + 1][j].rgbtGreen + image[i + 1][j + 1].rgbtGreen;
                sumBlue = image[i - 1][j].rgbtBlue + image[i - 1][j + 1].rgbtBlue + image[i][j].rgbtBlue +
                          image[i][j + 1].rgbtBlue + image[i + 1][j].rgbtBlue + image[i + 1][j + 1].rgbtBlue;
                N = 6.0;
            }
            else if (j == (width - 1))
            {
                // right edge (no j + 1)
                sumRed = image[i - 1][j - 1].rgbtRed + image[i - 1][j].rgbtRed + image[i][j - 1].rgbtRed + image[i][j].rgbtRed +
                         image[i + 1][j - 1].rgbtRed + image[i + 1][j].rgbtRed;
                sumGreen = image[i - 1][j - 1].rgbtGreen + image[i - 1][j].rgbtGreen + image[i][j - 1].rgbtGreen +
                           image[i][j].rgbtGreen + image[i + 1][j - 1].rgbtGreen + image[i + 1][j].rgbtGreen;
                sumBlue = image[i - 1][j - 1].rgbtBlue + image[i - 1][j].rgbtBlue + image[i][j - 1].rgbtBlue +
                          image[i][j].rgbtBlue + image[i + 1][j - 1].rgbtBlue + image[i + 1][j].rgbtBlue;
                N = 6.0;
            }
            else
            {
                // use all 8 nieghbors
                sumRed = image[i - 1][j - 1].rgbtRed + image[i - 1][j].rgbtRed + image[i - 1][j + 1].rgbtRed +
                         image[i][j - 1].rgbtRed + image[i][j].rgbtRed + image[i][j + 1].rgbtRed + image[i + 1][j - 1].rgbtRed +
                         image[i + 1][j].rgbtRed + image[i + 1][j + 1].rgbtRed;
                sumGreen = image[i - 1][j - 1].rgbtGreen + image[i - 1][j].rgbtGreen + image[i - 1][j + 1].rgbtGreen +
                           image[i][j - 1].rgbtGreen + image[i][j].rgbtGreen + image[i][j + 1].rgbtGreen +
                           image[i + 1][j - 1].rgbtGreen + image[i + 1][j].rgbtGreen + image[i + 1][j + 1].rgbtGreen;
                sumBlue = image[i - 1][j - 1].rgbtBlue + image[i - 1][j].rgbtBlue + image[i - 1][j + 1].rgbtBlue +
                          image[i][j - 1].rgbtBlue + image[i][j].rgbtBlue + image[i][j + 1].rgbtBlue +
                          image[i + 1][j - 1].rgbtBlue + image[i + 1][j].rgbtBlue + image[i + 1][j + 1].rgbtBlue;
                N = 9.0;
            }

            // sumRed = round(sumRed/N);
            // sumGreen = round(sumGreen/N);
            // sumBlue = round(sumBlue/N);

            sumRed = sumRed / N;
            sumGreen = sumGreen / N;
            sumBlue = sumBlue / N;

            if (sumRed > 255.0)
                sumRed = 255.0;
            if (sumGreen > 255.0)
                sumGreen = 255.0;
            if (sumBlue > 255.0)
                sumBlue = 255.0;

            sumRed = round(sumRed);
            sumGreen = round(sumGreen);
            sumBlue = round(sumBlue);

            // once iterated through all cells, calculate the average and assign to tmp
            tmp[i][j].rgbtRed = sumRed;
            tmp[i][j].rgbtGreen = sumGreen;
            tmp[i][j].rgbtBlue = sumBlue;
        }
    }

    // Second loop through all cells to re-assign values from tmp
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = tmp[i][j];
        }
    }

    return;
}
