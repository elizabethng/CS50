import sys


def main():
    # Request cc number - easiest to get it as a string
    num = input("Number: ")

    # length of cc number is 13, 15, 16
    # we can assume all numeric
    # need to double check, this might not be how implementation is desired
    while len(num) < 1:
        num = input("Number: ")
    # Check the number
    checksum(num)


def dig_sum(num):
    """Function to add together digits of a number"""
    sum = 0
    num = str(num)
    for i in range(len(num)):
        sum += int(num[i])
    return sum


def checksum(num):
    # Implement Luhn's algorithm
    # 1) Multiply every other digit by 2, starting with the number’s
    #    second-to-last digit, and then add those products’ digits together.
    # 2) Add the sum to the sum of the digits that weren’t multiplied by 2.
    # 3) If the total’s last digit is 0 (or, put more formally, if the total
    #    modulo 10 is congruent to 0), the number is valid!
    sum = 0
    rev = num[::-1]  # reversed string
    for i in range(len(rev)):
        if i % 2 == 0:
            sum += int(rev[i])
        else:
            sum += dig_sum(2 * int(rev[i]))

    # Check type
    if sum % 10 != 0:
        print("INVALID")
    elif len(num) == 15 and num[0] == "3" and (num[1] == "4" or num[1] == "7"):
        # Amex has 15 digits and starts with 34 or 37
        print("AMEX")
    elif (len(num) == 16 or len(num) == 13) and num[0] == "4":
        # Visa has 13 or 16 length and starts with 4
        print("VISA")
    elif len(num) == 16 and int(num[0:2]) in range(51, 56):
        # MasterCard has 16 digits and start with 51, 52, 53, 54, or 55
        print("MASTERCARD")
    else:
        print("INVALID")

    return sum


if __name__ == "__main__":
    main()
