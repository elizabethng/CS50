#include <cs50.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

int check_word(string guess, int wordsize, int status[], string choice);

int main(void)
{
    int wordsize = 5;
    string guess;
    guess = get_string("Input a %i-letter word:", wordsize);
    string choice = "chaos";
    int status[5] = {0};
    int out;

    out = check_word(guess, wordsize, status, choice);
    printf("Actual word:          %s\n", choice);
    printf("Score: %i\n", out);

    for (int i = 0; i < wordsize; i++)
    {
        printf("%i\n", status[i]);
    }
}

int check_word(string guess, int wordsize, int status[], string choice)
{
    int score = 0; // running score (2 for exact, 1 for close, 0 for no match)

    // compare guess to choice and score points as appropriate, storing points in status
    // iterate over each letter of the guess
    for (int i = 0; i < wordsize; i++)
    {
        // compare the current guess letter to the current choice letter
        // check the exact position case
        if (guess[i] == choice[i])
        {
            // if they're the same position in the word,
            // score EXACT points (green) and break so you don't compare that letter further
            score = score + 2;
            status[i] = 2;
        }
        else
        {
            // otherwise, loop through the rest of the choice word to see if
            // the guess letter is elsewhere in the word
            for (int j = 0; j < wordsize; j++)
            {
                if (guess[i] == choice[j])
                {
                    // if it's in the word, but not the right spot, score CLOSE point (yellow)
                    score = score + 1;
                    status[i] = 1;
                }
            }
        }
    }

    return score;
}