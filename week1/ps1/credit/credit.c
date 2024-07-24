#include <cs50.h>
#include <stdio.h>

long get_cc(void);
int dig_sum(int);
int checksum(long num);
int index(long, int);

int main(void)
{
    // Request cc number
    long num = get_cc();

    // Check the type
    checksum(num);
}

long get_cc(void)
{
    // request cc number until a number input is received
    long num;
    do
    {
        num = get_long("Number: ");
    }
    while (num < 1);

    return (num);
}


int checksum(long num)
{
    // Implement Luhn's algorithm
    int i = 0;
    int digit;
    long res = num;
    long sum = 0;

    while (res > 0)
    {
        i++;              // digit index
        digit = res % 10; // extract digit
        res = res / 10;   // move to next digit

        // Odd index digits are added (odd numbers have remainder)
        // Even index digits are multipled by 2 and added
        if (i % 2 == 1)
        {
            sum = sum + digit;
        }
        else
        {
            sum = sum + dig_sum(2 * digit);
        }
    }

    // Get second digit to check for card type
    int sec = index(num, i - 1);

    // Check that the number is valid, and if so
    // determine what type of card it is
    if (sum % 10 == 0)
    {
        if (((digit == 3) & (sec == 4 || sec == 7)) & (i == 15))
        {
            printf("AMEX\n");
        }
        else if ((digit == 4) & (i == 13 || i == 16))
        {
            printf("VISA\n");
        }
        else if (((digit == 5) & (0 < sec & sec < 6)) & (i == 16))
        {
            printf("MASTERCARD\n");
        }
        else
        {
            printf("INVALID\n");
        }
    }
    else
    {
        printf("INVALID\n");
    }

    return (sum);
}

// Function to add together digits of a number
// (max value of num in this exercise is 2*19)
int dig_sum(int num)
{
    int sum = 0;
    while (num > 0)
    {
        sum = sum + num % 10;
        num = num / 10;
    }
    return (sum);
}

// Negative index function
// Counting backwards, get value in ith slot
int index(long num, int loc)
{
    int val;

    for (int i  = 0; i < loc; i++)
    {
        val = num % 10; // extract digit
        num = num / 10;   // move to next digit
    }
    return (val);
}