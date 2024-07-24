from cs50 import get_string


def main():
    # Prompt user for a line of text
    text = get_string("Text: ")

    l = count_letters(text)
    w = count_words(text)
    s = count_sentences(text)

    # Calculate the grade
    L = 100.0 * l / w
    S = 100.0 * s / w
    grade = round(0.0588 * L - 0.296 * S - 15.8)

    if grade < 1:
        print("Before Grade 1")
    elif grade >= 16:
        print("Grade 16+")
    else:
        print(f"Grade {grade}")


def count_letters(text):
    """
    Calculate total number of letters
    (excluding punctuation, numbers, spaces, etc.)
    """
    l = 0
    for i in range(len(text)):
        if text[i].isalpha():
            l += 1
    return l


def count_words(text):
    """
    Count number of words
    - Any sequence of characters separated by a space is a word
    - Letters separated by hyphens are still one word
    """
    # try simplest implementation counting spaces!
    w = 1
    for i in range(len(text)):
        if text[i] == " ":
            w += 1
    return w


def count_sentences(text):
    """
    Count number of sentences
    Assume any ., !, or ? is the terminator of a sentence
    """
    s = 0
    for i in range(len(text)):
        if text[i] in [".", "!", "?"]:
            s += 1
    return s


if __name__ == "__main__":
    main()
