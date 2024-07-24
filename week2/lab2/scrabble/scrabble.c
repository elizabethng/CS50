#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

// Points assigned to each letter of the alphabet
int POINTS[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

int compute_score(string word);

int main(void)
{
    // Get input words from both players
    string word1 = get_string("Player 1: ");
    string word2 = get_string("Player 2: ");

    // Score both words
    int score1 = compute_score(word1);
    int score2 = compute_score(word2);

    // Print the winner
    printf("Score 1: %i\n", score1);
    printf("Score 2: %i\n", score2);

    if (score1 > score2)
    {
        printf("Player 1 wins!\n");
    }
    else if (score2 > score1)
    {
        printf("Player 2 wins!\n");
    }
    else
    {
        printf("Tie!\n");
    }
}

int compute_score(string word)
{
    // Compute and return score for string

    // get the length of the word
    int length = strlen(word);

    // placeholder for score (note look-up array is in the global environment)
    int score = 0;

    // loop through the characters of word
    for (int i = 0; i < length; i++)
    {
        // get current letter to evaluate
        int letter = word[i];

        // cases for each case
        if (letter > 64 & letter < 91) // can replace with isupper()
        {
            // upper case A = 65; Z = 90
            score += POINTS[letter - 65]; // can replace 65 with 'A'
        }
        else if (letter > 96 & letter < 123) // can replace with islower()
        {
            // lower case a = 97; z = 122
            score += POINTS[letter - 97]; // can replace 97 with 'a'
        }
    }

    return (score);
}
