#include <cs50.h>
#include <stdio.h>

int dig_sum(int);

int main(void)
{
    long num = get_long("Number: ");
    int digit;
    int i = 0;
    long sum = 0;
    while(num > 0)
    {
        i++;              // digit index
        digit = num % 10; // extract digit
        num = num / 10;   // move to next digit

        printf("index: %i\n", i);
        printf("digit: %i\n", digit);
        printf("new num: %li\n", num);
        printf("before sum: %li\n", sum);

        // Odd index digits are added (odd numbers have remainder)
        // Even index digits are multipled by 2 and added
        if (i % 2 == 1)
        {
            sum = sum + digit;
            printf("odd digit \n");
        }
        else
        {
            sum = sum + dig_sum(2 * digit);
            printf("even digit \n");
        }
        printf("after sum: %li\n ------- \n", sum);
    }

    // check that last digit is zero

    int checksum = sum % 10;

    if(checksum == 0)
    {
        printf("VALID: %i\n", checksum);
    }
    else
    {
        printf("INVALID: %i\n", checksum);
    }
return(0);
}

// Function to add together digits of a number
// (max value of num in this exercise is 2*19)
int dig_sum(int num)
{
    int sum = 0;
    while(num > 0){
        sum = sum + num % 10;
        num = num / 10;
    }
    return(sum);
}

// get first two digits
