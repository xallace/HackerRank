#!/bin/python3

import re

# Read N (rows) and M (columns)
first_multiple_input = input().rstrip().split()
n = int(first_multiple_input[0])
m = int(first_multiple_input[1])

matrix = []

# Read the matrix rows
for _ in range(n):
    matrix_item = input()
    matrix.append(matrix_item)

# 1. Read the matrix column by column (top to bottom, left to right)
decoded_chars = []
for col in range(m):
    for row in range(n):
        decoded_chars.append(matrix[row][col])

decoded_string = "".join(decoded_chars)

# 2. Regex substitution
# (?<=[A-Za-z0-9])  : Lookbehind ensuring the preceding character is alphanumeric
# [^A-Za-z0-9]+     : Matches one or more non-alphanumeric characters
# (?=[A-Za-z0-9])   : Lookahead ensuring the succeeding character is alphanumeric
pattern = r"(?<=[A-Za-z0-9])[^A-Za-z0-9]+(?=[A-Za-z0-9])"

# Replace the matched non-alphanumeric sequences with a single space
final_string = re.sub(pattern, " ", decoded_string)

print(final_string)
