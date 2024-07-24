#include <cs50.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

// Take text and determine it's grade level

// [X] Implement your program in a file called readability.c in a directory called readability.
// [X] Your program must prompt the user for a string of text using get_string.
// [X] Your program should count the number of letters, words, and sentences in the text.
//     You may assume that a letter is any lowercase character from a to z or any uppercase character from A to Z,
//     any sequence of characters separated by spaces should count as a word,
//     and that any occurrence of a period, exclamation point, or question mark indicates the end of a sentence.
// [X] Your program should print as output "Grade X" where X is the grade level computed by the Coleman-Liau formula,
//     rounded to the nearest integer.
// [X] If the resulting index number is 16 or higher (equivalent to or greater than a senior undergraduate
//     reading level), your program should output "Grade 16+" instead of giving the exact index number.
//     If the index number is less than 1, your program should output "Before Grade 1".

// Functions to obtain calculation components
int count_letters(string text);
int count_words(string text);
int count_sentences(string text);

int main(void)
{
    // Prompt user for a line of text
    string text = get_string("Text: ");

    // Get and print # letters
    int l = count_letters(text);
    // printf("%i Letters\n", l);

    // Get and print # words
    int w = count_words(text);
    // printf("%i Words\n", w);

    // Get and print # sentences
    int s = count_sentences(text);
    // printf("%i Sentences\n", s);

    // Calculate the grade
    float L = 100.0 * l / w;
    float S = 100.0 * s / w;
    float index = 0.0588 * L - 0.296 * S - 15.8;

    // round to the nearest integer to get the grade level
    int grade = round(index);

    if (grade < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (grade >= 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", grade);
    }
}

// Calculate total number of letters
// (excluding punctuation, numbers, spaces, etc.)
int count_letters(string text)
{
    int l = 0; // letter counter
    for (int i = 0; i < strlen(text); i++)
    {
        if (isalpha(text[i]))
        {
            l++;
        }
    }
    return (l);
}

// Calculate total number of words
// any sequence of characters separated by a space is a word
// letters separated by hyphens are still one word
// Assumptions
//    will contain at least one word;
//    will not start or end with a space; and
//    will not have multiple spaces in a row.
// APPROACH
// loop through each character
// if a letter is detected
// then keep iterating as long as letter or -
// then when false, check if it is a space? what about if just one word is supplied?
// until another character or the end of word
int count_words(string text)
{
    int w = 0; // word counter
    for (int i = 0; i < strlen(text); i++)
    {
        // check if we are on a letter
        // might want to an a or here for "-"
        if (isalpha(text[i]))
        {
            while (isalpha(text[i]) | (text[i] == '-') | (text[i] == '\''))
            {
                // if inside a word, keep iterating
                i++;
            }
            // if were're on a letter, keep going through them
            // until we get to anything that isn't a letter or -
            // 1. a space (i.e., isspace())
            // 2. punctuation that isn't - (i.e., ispunct)
            // 3. the end of the word
            // then increment our word counter
            w++;
        }
        // otherwise get the next char
    }
    return (w);
}

// Function to calculate number of sentences
// Assume any ., !, or ? is the terminator of a sentence
// If none of these are present, should it still count as a sentence?
int count_sentences(string text)
{
    int s = 0; // sentence counter
    for (int i = 0; i < strlen(text); i++)
    {
        if ((text[i] == '.') | (text[i] == '!') | (text[i] == '?'))
        {
            s++;
        }
    }
    return (s);
}