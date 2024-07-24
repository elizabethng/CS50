#include <cs50.h>
#include <stdio.h>
#include <math.h>

int main(void)
{
    // TODO: Prompt for start size
    int start;
    do
    {
        start = get_int("Start size: ");
    }
    while (start < 9);

    // TODO: Prompt for end size
    int end;
    do
    {
        end = get_int("End size: ");
    }
    while (end < start);

    // TODO: Calculate number of years until we reach threshold
    // N_t+1 = N_t + N_t/3 - N_t/4 = N_t*(1 + 1/3 - 1/4) = N_t * (13/12)
    int t = 0;
    while (start < end)
    {
        start = start + start / 3 - start / 4;
        t++;
    }

    // TODO: Print number of years
    printf("Years: %i\n", t);
}
