from cs50 import get_int


def main():
    # Ask for size in [1:8]
    n = get_int("Size: ")
    while n < 1 or n > 8:
        n = get_int("Size: ")

    # Make a row
    for i in range(n):
        # Make a column
        # Make padding spaces
        for j in range(n - i - 1):
            print(" ", end="")

        # Make blocks
        for j in range(i + 1):
            print("#", end="")

        # Central spacer
        print("  ", end="")

        # Make second side blocks
        for j in range(i + 1):
            print("#", end="")

        # Next line
        print("")


if __name__ == "__main__":
    main()
