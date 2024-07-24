#include <cs50.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

// Max voters and candidates
#define MAX_VOTERS 100
#define MAX_CANDIDATES 9

// preferences[i][j] is jth preference for voter i
int preferences[MAX_VOTERS][MAX_CANDIDATES];

// Candidates have name, vote count, eliminated status
typedef struct
{
    string name;
    int votes;
    bool eliminated;
} candidate;

// Array of candidates
candidate candidates[MAX_CANDIDATES];

// Numbers of voters and candidates
int voter_count;
int candidate_count;

// Function prototypes
bool vote(int voter, int rank, string name);
void tabulate(void);
bool print_winner(void);
int find_min(void);
bool is_tie(int min);
void eliminate(int min);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: runoff [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX_CANDIDATES)
    {
        printf("Maximum number of candidates is %i\n", MAX_CANDIDATES);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
        candidates[i].eliminated = false;
    }

    voter_count = get_int("Number of voters: ");
    if (voter_count > MAX_VOTERS)
    {
        printf("Maximum number of voters is %i\n", MAX_VOTERS);
        return 3;
    }

    // Keep querying for votes
    for (int i = 0; i < voter_count; i++)
    {

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            // Record vote, unless it's invalid
            if (!vote(i, j, name))
            {
                printf("Invalid vote.\n");
                return 4;
            }
        }

        printf("\n");
    }

    // Keep holding runoffs until winner exists
    while (true)
    {
        // Calculate votes given remaining candidates
        tabulate();

        // Check if election has been won
        bool won = print_winner();
        if (won)
        {
            break;
        }

        // Eliminate last-place candidates
        int min = find_min();
        bool tie = is_tie(min);

        // If tie, everyone wins
        if (tie)
        {
            for (int i = 0; i < candidate_count; i++)
            {
                if (!candidates[i].eliminated)
                {
                    printf("%s\n", candidates[i].name);
                }
            }
            break;
        }

        // Eliminate anyone with minimum number of votes
        eliminate(min);

        // Reset vote counts back to zero
        for (int i = 0; i < candidate_count; i++)
        {
            candidates[i].votes = 0;
        }
    }
    return 0;
}

// Record preference if vote is valid
// first called on line 75
bool vote(int voter, int rank, string name)
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i].name) == 0)
        {
            preferences[voter][rank] = i;
            return true;
        }
    }
    return false;
}

// Tabulate votes for non-eliminated candidates
// Each voter votes for their top-ranked candidate that is not eliminated
// called on line 90 inside a while loop
void tabulate(void)
{
    // Iterate through each voter
    for (int i = 0; i < voter_count; i++)
    {
        // Iterate through each rank of candidate
        // We assume that every candidate will be ranked
        for (int j = 0; j < candidate_count; j++)
        {
            // Get the index of the candidate in the given rank slot
            // and check whether that choice candidate has been eliminated
            int can_tmp = preferences[i][j];
            bool can_status = candidates[can_tmp].eliminated;

            // If not eliminated, increment the vote counter by 1 and
            // move to the next voter's selection
            if (can_status == false)
            {
                candidates[can_tmp].votes++;
                break;
            }
        }
    }
    return;
}

// Print the winner of the election, if there is one
// Called on line 93
// Check if any candidate has more than 50% of vote
// shouldn't need to worry about eliminated candidates?
bool print_winner(void)
{
    int threshold = voter_count / 2.0;
    for (int j = 0; j < candidate_count; j++)
    {
        if (candidates[j].votes > threshold)
        {
            printf("%s\n", candidates[j].name);
            return true;
        }
    }
    return false;
}

// Return the minimum number of votes any remaining candidate has
// Skip any eliminated candidates!
// Called Line 101
int find_min(void)
{
    // initialize starting comparison to maximum number of votes
    int min = MAX_VOTERS;

    // include first comp to check for elimination
    for (int j = 0; j < candidate_count; j++)
    {
        // only include candidates that are not eliminated
        if (candidates[j].eliminated == false)
        {
            // keep the next value if it is smaller
            if (candidates[j].votes < min)
            {
                min = candidates[j].votes;
            }
        }
    }
    return min;
}

// Return true if the election is tied between all candidates, false otherwise
// Tie occurs if all reminaing candidates have the same (minimum) score
// called Line 102
bool is_tie(int min)
{
    for (int j = 0; j < candidate_count; j++)
    {
        // only include candidates that are not eliminated
        if (candidates[j].eliminated == false)
        {
            // If a value is not min (greater than) then return false (no tie)
            if (candidates[j].votes != min)
            {
                return false;
            }
        }
    }
    // if all the scores are min then there is a tie
    return true;
}

// Eliminate the candidate (or candidates) in last place
void eliminate(int min)
{
    for (int j = 0; j < candidate_count; j++)
    {
        // only include candidates that are not eliminated
        if (candidates[j].eliminated == false)
        {
            // If their votes == min, then change to eliminated
            if (candidates[j].votes == min)
            {
                candidates[j].eliminated = true;
            }
        }
    }
    return;
}