#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // Prompt picking a number [1, 8]
    int n;
    do
    {
        n = get_int("Height:");
    }
    while (n < 1 || n > 8);

    // Make a row
    for (int i = 0; i < n; i++)
    {
        // Make a column

        // Make padding spaces
        for (int j = 0; j < (n - i - 1); j++)
        {
            printf(" ");
        }
        // Make blocks
        for (int j = 0; j < i + 1; j++)
        {
            printf("#");
        }
        printf("  ");
        // Make second side blocks
        for (int j = 0; j < i + 1; j++)
        {
            printf("#");
        }
        // Next line
        printf("\n");
    }
}