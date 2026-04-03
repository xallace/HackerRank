#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// Segment Tree node to store max value and lazy propagation tag
struct Node {
    long long max_val;
    long long lazy;
};

vector<Node> tree;

// Build the initial segment tree over the unique sorted deadlines
void build(int node, int L, int R, const vector<int>& S) {
    tree[node].lazy = 0;
    if (L == R) {
        // Initially, the sum of M_x for x <= S[L] is 0
        // So V(S[L]) = -S[L]
        tree[node].max_val = -S[L];
        return;
    }
    int mid = L + (R - L) / 2;
    build(2 * node, L, mid, S);
    build(2 * node + 1, mid + 1, R, S);
    tree[node].max_val = max(tree[2 * node].max_val, tree[2 * node + 1].max_val);
}

// Range update with lazy propagation
void update(int node, int L, int R, int ql, int qr, long long val) {
    if (tree[node].lazy != 0) {
        tree[node].max_val += tree[node].lazy;
        if (L != R) {
            tree[2 * node].lazy += tree[node].lazy;
            tree[2 * node + 1].lazy += tree[node].lazy;
        }
        tree[node].lazy = 0;
    }

    if (ql > R || qr < L) {
        return;
    }

    if (ql <= L && R <= qr) {
        tree[node].max_val += val;
        if (L != R) {
            tree[2 * node].lazy += val;
            tree[2 * node + 1].lazy += val;
        }
        return;
    }

    int mid = L + (R - L) / 2;
    update(2 * node, L, mid, ql, qr, val);
    update(2 * node + 1, mid + 1, R, ql, qr, val);
    tree[node].max_val = max(tree[2 * node].max_val, tree[2 * node + 1].max_val);
}

int main() {
    // Untie I/O streams for competitive programming speed
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    if (!(cin >> T)) return 0;

    // Read all tasks first to allow coordinate compression on deadlines
    vector<pair<int, int>> tasks(T);
    vector<int> S;
    S.reserve(T);

    for (int i = 0; i < T; ++i) {
        cin >> tasks[i].first >> tasks[i].second;
        S.push_back(tasks[i].first);
    }

    // Coordinate compression for the segment tree
    sort(S.begin(), S.end());
    S.erase(unique(S.begin(), S.end()), S.end());

    int n_unique = S.size();
    
    // Allocate segment tree space
    tree.resize(4 * n_unique + 1);

    // Initialize the segment tree
    if (n_unique > 0) {
        build(1, 0, n_unique - 1, S);
    }

    // Process tasks sequentially
    for (int i = 0; i < T; ++i) {
        int D = tasks[i].first;
        long long M = tasks[i].second;
        
        // Find the compressed index for the deadline D
        int idx = lower_bound(S.begin(), S.end(), D) - S.begin();
        
        // Add M to all deadlines >= D
        update(1, 0, n_unique - 1, idx, n_unique - 1, M);
        
        // The max overshoot is bounded below by 0
        long long ans = max(0LL, tree[1].max_val);
        cout << ans << "\n";
    }

    return 0;
}
