#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// Candidates have name and vote count
typedef struct
{
    string name;
    int votes;
} candidate;

// Array of candidates
candidate candidates[MAX];

// Number of candidates
int candidate_count;

// Function prototypes
bool vote(string name);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: plurality [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
    }

    int voter_count = get_int("Number of voters: ");

    // Loop over all voters
    for (int i = 0; i < voter_count; i++)
    {
        string name = get_string("Vote: ");

        // Check for invalid vote
        if (!vote(name))
        {
            printf("Invalid vote.\n");
        }
        // adding this
        // printf("Alice votes: %d\n", candidates[0].votes);
        // printf("Bob votes: %d\n", candidates[1].votes);
    }

    // Display winner of election
    print_winner();
}

// Update vote totals given a new vote
// given a name, iterate through the candidate list
// if a match is found, increment that candidate's tally and return/break?
// if no match is found, return false
// CURRENTLY - vote isn't incrementing correclty. need to check strcomp
bool vote(string name)
{
    for (int i = 0; i < candidate_count; i++)
    {
        // printf("comparing %s and %s\n", name, candidates[i].name);
        // bool comp;
        // comp = strcmp(name, candidates[i].name) == 0;
        // printf("%s\n", comp ? "true" : "false");

        if (strcmp(name, candidates[i].name) == 0)
        {
            candidates[i].votes++;
            return true;
        }
    }
    return false;
}

// Print the winner (or winners) of the election
void print_winner(void)
{
    // 1. Find maxmum vote value
    int max;
    max = candidates[0].votes;                // initialize with first value
    for (int i = 1; i < candidate_count; i++) // skip comparing first value to itself
    {
        if (candidates[i].votes > max)
        {
            max = candidates[i].votes; // keep next number if it is larger
        }
    }
    // printf("max votes: %i\n", max);

    // 2. Print any winners who have the max number of votes
    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].votes == max)
        {
            printf("%s\n", candidates[i].name);
        }
    }
    return;
}
