import sys

def solve():
    input_data = sys.stdin.read().split()
    if not input_data:
        return
    
    n = int(input_data[0])
    q_count = int(input_data[1])
    s = input_data[2]
    
    # ---------------------------------------------------------
    # 1. Suffix Automaton (SAM) Construction
    # ---------------------------------------------------------
    MAX_NODES = 2 * n + 5
    sam_len = [0] * MAX_NODES
    sam_link = [-1] * MAX_NODES
    
    # Flat 2D array avoids slow dictionary hashes
    sam_next = [[-1] * 26 for _ in range(MAX_NODES)]
    
    last = 0
    sz = 1
    pos_to_node = [0] * n
    
    for i in range(n):
        char_idx = ord(s[i]) - 97
        cur = sz
        sz += 1
        sam_len[cur] = sam_len[last] + 1
        p = last
        
        while p != -1 and sam_next[p][char_idx] == -1:
            sam_next[p][char_idx] = cur
            p = sam_link[p]
            
        if p == -1:
            sam_link[cur] = 0
        else:
            q = sam_next[p][char_idx]
            if sam_len[p] + 1 == sam_len[q]:
                sam_link[cur] = q
            else:
                clone = sz
                sz += 1
                sam_len[clone] = sam_len[p] + 1
                sam_link[clone] = sam_link[q]
                # Fast list copy
                sam_next[clone] = list(sam_next[q])
                
                while p != -1 and sam_next[p][char_idx] == q:
                    sam_next[p][char_idx] = clone
                    p = sam_link[p]
                sam_link[q] = clone
                sam_link[cur] = clone
        last = cur
        pos_to_node[i] = cur

    # ---------------------------------------------------------
    # 2. Range Binary Indexed Tree (BIT)
    # ---------------------------------------------------------
    bit1 = [0] * (n + 5)
    bit2 = [0] * (n + 5)

    def bit_range_add(l, r, val):
        if l > r: return
        
        i = l
        while i <= n:
            bit1[i] += val
            i += i & (-i)
            
        i = r + 1
        while i <= n:
            bit1[i] -= val
            i += i & (-i)
            
        v1 = val * l
        i = l
        while i <= n:
            bit2[i] += v1
            i += i & (-i)
            
        v2 = -val * (r + 1)
        i = r + 1
        while i <= n:
            bit2[i] += v2
            i += i & (-i)

    def bit_prefix_sum(idx):
        if idx <= 0: return 0
        s1 = 0
        s2 = 0
        i = idx
        while i > 0:
            s1 += bit1[i]
            s2 += bit2[i]
            i -= i & (-i)
        return s1 * (idx + 1) - s2

    # ---------------------------------------------------------
    # 3. Iterative Link-Cut Tree (LCT)
    # ---------------------------------------------------------
    lct_parent = list(sam_link) 
    lct_ch0 = [0] * sz
    lct_ch1 = [0] * sz
    lct_val = [0] * sz
    lct_lazy = [0] * sz
    lct_min_len = [0] * sz
    lct_splay_min = [0] * sz
    
    for i in range(1, sz):
        lct_min_len[i] = sam_len[sam_link[i]] + 1
        lct_splay_min[i] = lct_min_len[i]
        
    splay_stack = [0] * sz

    def rotate(x):
        y = lct_parent[x]
        z = lct_parent[y]
        is_right_x = (lct_ch1[y] == x)
        
        # Inline is_root(y)
        if z != 0 and lct_ch0[z] == y:
            lct_ch0[z] = x
        elif z != 0 and lct_ch1[z] == y:
            lct_ch1[z] = x
            
        lct_parent[x] = z
        
        if is_right_x:
            lct_ch1[y] = lct_ch0[x]
            if lct_ch0[x]:
                lct_parent[lct_ch0[x]] = y
            lct_ch0[x] = y
        else:
            lct_ch0[y] = lct_ch1[x]
            if lct_ch1[x]:
                lct_parent[lct_ch1[x]] = y
            lct_ch1[x] = y
            
        lct_parent[y] = x
        
        # Inline push_up(y)
        res_y = lct_min_len[y]
        ly = lct_ch0[y]
        ry = lct_ch1[y]
        if ly and lct_splay_min[ly] < res_y: res_y = lct_splay_min[ly]
        if ry and lct_splay_min[ry] < res_y: res_y = lct_splay_min[ry]
        lct_splay_min[y] = res_y
        
        # Inline push_up(x)
        res_x = lct_min_len[x]
        lx = lct_ch0[x]
        rx = lct_ch1[x]
        if lx and lct_splay_min[lx] < res_x: res_x = lct_splay_min[lx]
        if rx and lct_splay_min[rx] < res_x: res_x = lct_splay_min[rx]
        lct_splay_min[x] = res_x

    def splay(x):
        # 1. Inline push_down from root to x
        top = 0
        curr = x
        splay_stack[0] = curr
        while True:
            p = lct_parent[curr]
            if p == 0 or (lct_ch0[p] != curr and lct_ch1[p] != curr):
                break
            top += 1
            curr = p
            splay_stack[top] = curr
            
        while top >= 0:
            curr = splay_stack[top]
            top -= 1
            lz = lct_lazy[curr]
            if lz:
                l = lct_ch0[curr]
                r = lct_ch1[curr]
                if l:
                    lct_val[l] = lz
                    lct_lazy[l] = lz
                if r:
                    lct_val[r] = lz
                    lct_lazy[r] = lz
                lct_lazy[curr] = 0
                
        # 2. Iterative Splay Rotation Loop
        while True:
            y = lct_parent[x]
            if y == 0 or (lct_ch0[y] != x and lct_ch1[y] != x):
                break
            z = lct_parent[y]
            y_is_root = (z == 0 or (lct_ch0[z] != y and lct_ch1[z] != y))
            
            if not y_is_root:
                if (lct_ch1[y] == x) == (lct_ch1[z] == y):
                    rotate(y)
                else:
                    rotate(x)
            rotate(x)

    # ---------------------------------------------------------
    # 4. Offline Query Processing
    # ---------------------------------------------------------
    queries_by_r = [[] for _ in range(n + 1)]
    q_ptr = 3
    for i in range(q_count):
        l = int(input_data[q_ptr]) + 1
        r = int(input_data[q_ptr + 1]) + 1
        queries_by_r[r].append((l, i))
        q_ptr += 2

    results = [0] * q_count

    # Sweep-line across the string
    for r in range(1, n + 1):
        x = pos_to_node[r - 1]
        
        # Access(x) in LCT and dynamically update the Fenwick Tree
        curr = x
        right_child = 0
        while curr > 0:
            splay(curr)
            old_color = lct_val[curr]
            
            if old_color > 0:
                max_l = sam_len[curr]
                min_l = lct_min_len[curr]
                l_child = lct_ch0[curr]
                if l_child and lct_splay_min[l_child] < min_l:
                    min_l = lct_splay_min[l_child]
                
                bit_range_add(old_color - max_l + 1, old_color - min_l + 1, -1)
                
            lct_ch1[curr] = right_child
            
            # Inline push_up(curr)
            res_curr = lct_min_len[curr]
            lc = lct_ch0[curr]
            rc = lct_ch1[curr]
            if lc and lct_splay_min[lc] < res_curr: res_curr = lct_splay_min[lc]
            if rc and lct_splay_min[rc] < res_curr: res_curr = lct_splay_min[rc]
            lct_splay_min[curr] = res_curr
            
            right_child = curr
            curr = lct_parent[curr]
            
        # Color the solid path from root to x with 'r'
        splay(x)
        lct_val[x] = r
        lct_lazy[x] = r
        
        # Add the new contribution
        max_l = sam_len[x]
        min_l = lct_splay_min[x]
        bit_range_add(r - max_l + 1, r - min_l + 1, 1)
        
        # Answer queries ending at r
        for l, q_idx in queries_by_r[r]:
            results[q_idx] = bit_prefix_sum(r) - bit_prefix_sum(l - 1)

    sys.stdout.write('\n'.join(map(str, results)) + '\n')

if __name__ == '__main__':
    solve()
