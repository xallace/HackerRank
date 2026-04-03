import sys

def get_z_array(s):
    """
    Computes the Z-array for a string s.
    z[i] is the length of the longest common prefix between s and s[i:].
    """
    n = len(s)
    z = [0] * n
    l, r = 0, 0
    for i in range(1, n):
        if i <= r:
            z[i] = min(r - i + 1, z[i - l])
        while i + z[i] < n and s[z[i]] == s[i + z[i]]:
            z[i] += 1
        if i + z[i] - 1 > r:
            l, r = i, i + z[i] - 1
    return z

def solve():
    """
    Solves the Save Humanity problem.
    Calculates LCP from the front and LCS from the back for every window.
    If LCP + LCS >= length - 1, it's a match (at most 1 mismatch).
    """
    input_data = sys.stdin.read().splitlines()
    if not input_data:
        return
    
    t = int(input_data[0])
    line_idx = 1
    
    results = []
    
    for _ in range(t):
        if line_idx >= len(input_data):
            break
            
        line = input_data[line_idx].split()
        if len(line) < 2:
            # Handle edge case where input might be formatted differently
            line_idx += 1
            continue
            
        p = line[0] # DNA (Patient)
        v = line[1] # Virus
        line_idx += 1
        
        n = len(p)
        m = len(v)
        
        if m > n:
            results.append("No Match!")
            continue
            
        # 1. Forward LCP: Find how many chars match starting from index i
        # string: virus + # + dna
        z_forward = get_z_array(v + "#" + p)
        lcp = z_forward[m + 1:]
        
        # 2. Backward LCS: Find how many chars match ending at index i+m-1
        # string: reverse(virus) + # + reverse(dna)
        z_backward = get_z_array(v[::-1] + "#" + p[::-1])
        lcs_rev = z_backward[m + 1:]
        
        matches = []
        for i in range(n - m + 1):
            pre = lcp[i]
            if pre >= m:
                # Exact match
                matches.append(str(i))
            else:
                # Check if suffix matches after the single mismatch
                # The index in the reversed lcs array for a window starting at i is (n - m - i)
                suf = lcs_rev[n - m - i]
                if pre + suf >= m - 1:
                    matches.append(str(i))
        
        if not matches:
            results.append("No Match!")
        else:
            results.append(" ".join(matches))
        
    sys.stdout.write("\n".join(results) + "\n")

if __name__ == '__main__':
    solve()
