#include <cs50.h>
#include <stdio.h>

int index(long, int);

int main(void)
{
    long num = get_long("Number: ");
    int loc = get_long("Index: ");

    int res;
    res = index(num, loc);
    printf("%i\n", res);
}

// Negative index function
// Counting backwards, get value in ith slot
int index(long num, int loc)
{
    int digit;

    for (int i  = 0; i < loc; i++)
    {
        digit = num % 10; // extract digit
        num = num / 10;   // move to next digit
    }
    printf("%i\n", digit);
    return (digit);
}