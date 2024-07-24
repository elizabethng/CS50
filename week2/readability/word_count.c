#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>


int main(void)
{
    // Prompt user for a line of text
    string text = get_string("Text: ");

    int w = 0; // word counter
    for (int i = 0; i < strlen(text); i++)
    {
        // check if we are on a letter
        // might want to an a or here for "-"
        if(isalpha(text[i]))
        {
            while(isalpha(text[i]) | (text[i] == '-'))
            {
                // if inside a word, keep iterating
                i++;
            }
            // if were're on a letter, keep going through them
            // until we get to anything that isn't a letter or -
            // 1. a space (i.e., isspace())
            // 2. punctuation that isn't - (i.e., ispunct)
            // 3. the end of the word
            w ++;
        }
        // otherwise get the next char
    }
    printf("%i\n", w);
    // return (w);
}