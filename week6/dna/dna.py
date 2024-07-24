import csv
from sys import argv


def main():
    # TODO: Check for command-line usage
    if len(argv) != 3:
        print(f"Usage: python dna.py [COUNTS.csv] [SEQUENCE.txt]")

    # TODO: Read database file into a variable
    # note this is a list of dictionaries
    database = []
    filename = argv[1]  # "week6/dna/databases/small.csv"
    with open(filename) as c:
        reader = csv.DictReader(c)
        for row in reader:
            database.append(row)

    # TODO: Read DNA sequence file into a variable
    filename = argv[2]  # "week6/dna/sequences/1.txt"
    with open(filename, "r") as f:
        dna = f.readline()
        f.close()

    # TODO: Find longest match of each STR in DNA sequence
    substring = list(database[0].keys())[1::]  # first element is name
    counts = {s: longest_match(dna, s) for s in substring}

    # TODO: Check database for matching profiles
    # All counts must match! and there is at most 1 match per
    match = "No match"
    for entry in database:
        check = [entry[s] == str(counts[s]) for s in substring]
        if all(check):
            match = entry["name"]
            break

    print(f"{match}")

    return 0


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):
        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:
            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


main()
