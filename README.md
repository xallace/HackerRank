# Morgan and a String

This project implements a solution to the "Morgan and a String" problem, which involves creating the lexicographically smallest string by comparing and merging two input strings.

## Problem Description

The problem involves two strings, A and B. The goal is to form the lexicographically smallest string possible by taking characters from the front of either string A or B in each step. At each step, we choose to take the next character from the string that would result in the lexicographically smaller outcome.

## How It Works

The algorithm uses a two-pointer approach with the following key steps:

1. Maintain two pointers, one for each string (A and B)
2. At each step, compare the remaining substrings starting from the current positions
3. When characters match at the current positions in both strings, continue comparing further ahead to make the optimal choice
4. Always choose the character that leads to the lexicographically smaller result

## Algorithm Details

- Time Complexity: O((n + m)^2) in the worst case, where n and m are the lengths of strings A and B respectively
- Space Complexity: O(n + m) for storing the result string

The solution handles edge cases where one string is a prefix of another or where strings have long sequences of identical characters.

## Usage

1. Run the script: `python Morgan-and-a-String`
2. Input the number of test cases (T)
3. For each test case, input two strings A and B
4. The program will output the lexicographically smallest merged string

## Example

Input:
```
1
JACK
DANIEL
```

Output:
```
DAJACKNIEL
```

## File Structure

- `Morgan-and-a-String`: Main Python script containing the solution

## Solution Approach

The algorithm compares the remaining parts of both strings at each step. When the characters at the current positions are the same, it continues to look ahead to determine which choice will result in a lexicographically smaller final string. This prevents making locally optimal choices that could lead to a globally suboptimal result.