// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// TODO: Choose number of buckets in hash table
// In division method hash function, helpful if hash table size is a prime number
// prime number from https://en.wikipedia.org/wiki/List_of_prime_numbers
const unsigned int N = 2851;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO
    // with hash function can go directly to the correct node in the dictionary
    // if the table element is null then I don't have check further?
    node *ptr = table[hash(word)];

    // check the first element
    while (ptr != NULL)
    {
        if (strcasecmp(ptr->word, word) == 0)
        {
            return true;
        }
        else
        {
            ptr = ptr->next;
        }
    }
    return false;
}

// Hashes word to a number
// Try implementing the division method described below
// https://www.geeksforgeeks.org/hash-functions-and-list-types-of-hash-functions/
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function: toupper(word[0]) - 'A'
    // sum the values in the word
    int k = 0;
    int i = 0;
    while (word[i] != '\0')
    {
        k += (toupper(word[i]) - 'A');
        i++;
    }

    return k % N;
}

// Loads dictionary into memory, returning true if successful, else false
// dictionary entries are stored in a hash table
bool load(const char *dictionary)
{
    // TODO
    // Allocate memory for the dictionary
    for (int i = 0; i < N; i++)
    {
        table[i] = malloc(sizeof(node));
        if (table[i] == NULL)
        {
            return 1;
        }
        else
        {
            // set the value of the next pointer and word to NULL
            // to avoid uninitialized value valgrind errors
            // https://cs50.stackexchange.com/questions/37174/pset5-speller-valgrind-conditional-jump-or-move-depends-on-uninitialized-values
            // https://cs50.stackexchange.com/questions/9260/how-to-fix-the-conditional-jump-or-move-depends-on-uninitialized-value-valgrin
            table[i]->word[0] = '\0';
            table[i]->next = NULL;
        }
    }

    // Open the file and check for success
    FILE *dict = fopen(dictionary, "r");
    if (dict == NULL)
    {
        return false;
    }

    // Read in each line of the dictionary
    char buffer[LENGTH + 2]; // maximum word length is LENGTH need +1 here for nul
    while (fgets(buffer, sizeof(buffer), dict) != NULL)
    {
        // Remove newline character from buffer
        buffer[strcspn(buffer, "\n")] = '\0';

        // Allocate node for word
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            return false;
        }
        // Put the word in the word slot, and NULL for the next pointer slot
        strcpy(n->word, buffer); // can't use n->word = buffer so need to copy the string over
        n->next = NULL;

        // Prepend node to list
        // HERE list is the list within an element of the array (hash table)
        n->next = table[hash(buffer)];
        table[hash(buffer)] = n;

        // QC print read ins
        /*
        printf("%i: ", hash(buffer));
        printf("%s, ", buffer);
        printf("%s\n", table[hash(buffer)]->word);
        */
    }

    // QC print all words in the dictionary
    /*
    for (int i = 0; i < N; i++)
    {
        node *ptr = table[i];
        while (ptr != NULL)
        {
            printf("WC %s\n", ptr->word);
            ptr = ptr->next;
        }
    }
    */

    fclose(dict);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
// need to move through all parts of dictionary and count them up
// dictionary is stored in table
unsigned int size(void)
{
    // TODO
    // only count elements that have words there!
    int size = 0;

    for (int i = 0; i < N; i++)
    {
        node *ptr = table[i];
        while (ptr != NULL)
        {
            if (ptr->word[0] != '\0')
            {
                size++;
            }
            ptr = ptr->next;
        }
    }

    // QC printf("Dictionary size: %i\n", size);
    return size;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO
    for (int i = 0; i < N; i++)
    {
        node *ptr = table[i];
        while (ptr != NULL)
        {
            node *next = ptr->next;
            free(ptr);
            ptr = next;
        }
    }

    return true;
}
