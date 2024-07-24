// Practice writing a function to find a max value

#include <cs50.h>
#include <stdio.h>

int max(int array[], int n);

int main(void)
{
    int n;
    do
    {
        n = get_int("Number of elements: ");
    }
    while (n < 1);

    int arr[n];

    for (int i = 0; i < n; i++)
    {
        arr[i] = get_int("Element %i: ", i);
    }

    printf("The max value is %i.\n", max(arr, n));
}

// TODO: return the max value
int max(int array[], int n)
{
    // initialize starting comparison
    int max;
    max = array[0];

    // can skip first value
    for (int i = 1; i < n; i++)
    {
        // keep the next value if it is larger
        if (array[i] > max)
        {
            max = array[i];
        }
    }
    return max;
}
