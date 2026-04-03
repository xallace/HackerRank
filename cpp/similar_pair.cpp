#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// Fenwick Tree (Binary Indexed Tree) for efficient range queries and updates
struct FenwickTree {
    int n;
    vector<int> bit;
    
    FenwickTree(int n) : n(n), bit(n + 1, 0) {}

    // Add 'val' to the element at index 'idx'
    void add(int idx, int val) {
        for (; idx <= n; idx += idx & -idx)
            bit[idx] += val;
    }

    // Get the prefix sum up to index 'idx'
    int query(int idx) {
        int sum = 0;
        for (; idx > 0; idx -= idx & -idx)
            sum += bit[idx];
        return sum;
    }

    // Get the sum in the range [l, r]
    int query(int l, int r) {
        if (l > r) return 0;
        return query(r) - query(l - 1);
    }
};

/*
 * Complete the 'similarPair' function below.
 *
 * The function is expected to return a LONG INTEGER.
 * The function accepts following parameters:
 * 1. INTEGER n
 * 2. INTEGER k
 * 3. 2D_INTEGER_ARRAY edges
 */
long long similarPair(int n, int k, vector<vector<int>> edges) {
    // adj will store the tree structure
    vector<vector<int>> adj(n + 1);
    vector<int> in_degree(n + 1, 0);

    for (const auto& edge : edges) {
        int u = edge[0]; // Parent
        int v = edge[1]; // Child
        adj[u].push_back(v);
        in_degree[v]++;
    }

    // Find the root of the tree (node with in-degree 0)
    int root = 1;
    for (int i = 1; i <= n; ++i) {
        if (in_degree[i] == 0) {
            root = i;
            break;
        }
    }

    FenwickTree bit(n);
    long long total_pairs = 0;

    // Lambda for DFS traversal using C++14/20 auto parameter for recursion
    auto dfs = [&](auto& self, int u) -> void {
        // Query the number of ancestors currently in the path 
        // that fall within the valid range [u - k, u + k]
        int left = max(1, u - k);
        int right = min(n, u + k);
        
        total_pairs += bit.query(left, right);
        
        // Add the current node to the active ancestor set
        bit.add(u, 1);
        
        // Traverse children
        for (int v : adj[u]) {
            self(self, v);
        }
        
        // Remove the current node from the active ancestor set as we backtrack
        bit.add(u, -1);
    };

    // Start DFS from the root
    dfs(dfs, root);

    return total_pairs;
}

// Minimal main function for local testing
int main() {
    int n, k;
    if (cin >> n >> k) {
        vector<vector<int>> edges(n - 1, vector<int>(2));
        for (int i = 0; i < n - 1; ++i) {
            cin >> edges[i][0] >> edges[i][1];
        }
        cout << similarPair(n, k, edges) << "\n";
    }
    return 0;
}
