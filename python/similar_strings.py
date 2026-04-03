import sys

def z_function(s):
    """
    Computes the Z-array for a string s.
    z[i] is the length of the longest common prefix between s and s[i:].
    Complexity: O(n)
    """
    n = len(s)
    z = [0] * n
    # z[0] is usually undefined or the full length of the string
    # For this specific problem, the prefix of s starting at 0 is the string itself
    z[0] = n
    
    # [l, r] make a window which matches with prefix of s
    l, r = 0, 0
    for i in range(1, n):
        if i <= r:
            # We are within a previously matched segment.
            # Use previously computed values to avoid redundant comparisons.
            z[i] = min(r - i + 1, z[i - l])
        
        # Attempt to extend the match beyond the current window
        while i + z[i] < n and s[z[i]] == s[i + z[i]]:
            z[i] += 1
            
        # If we found a match that extends beyond the current r, update the window
        if i + z[i] - 1 > r:
            l, r = i, i + z[i] - 1
            
    return z

def stringSimilarity(s):
    """
    Returns the sum of lengths of the longest common prefixes 
    between s and all its suffixes.
    """
    # The Z-array directly gives us these lengths for every index i
    z = z_function(s)
    return sum(z)

if __name__ == '__main__':
    # Using sys.stdin.read for faster input handling on large test cases
    input_data = sys.stdin.read().split()
    if not input_data:
        exit()
        
    t = int(input_data[0])
    results = []
    
    for i in range(1, t + 1):
        s = input_data[i]
        results.append(str(stringSimilarity(s)))
    
    # Print all results joined by newline for efficiency
    sys.stdout.write("\n".join(results) + "\n")
