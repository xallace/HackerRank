#pragma GCC optimize("O3,unroll-loops")
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

using namespace std;

const long long INF = 1000000000000000000LL;

// Structure to store city attributes
struct City {
    int x, y, h;
    long long p;
} cities[200005];

// Structure for K-D Tree Nodes
struct KDNode {
    int x, y;
    int min_x, max_x, min_y, max_y;
    long long dp, max_dp;
    int left, right, parent;
} nodes[200005];

int build_indices[200005];
long long group_dp[200005];

// Initialize and compute bounding boxes for a node based on its children
void pull_bbox(int u) {
    nodes[u].min_x = nodes[u].max_x = nodes[u].x;
    nodes[u].min_y = nodes[u].max_y = nodes[u].y;
    
    if (nodes[u].left) {
        int l = nodes[u].left;
        if (nodes[l].min_x < nodes[u].min_x) nodes[u].min_x = nodes[l].min_x;
        if (nodes[l].max_x > nodes[u].max_x) nodes[u].max_x = nodes[l].max_x;
        if (nodes[l].min_y < nodes[u].min_y) nodes[u].min_y = nodes[l].min_y;
        if (nodes[l].max_y > nodes[u].max_y) nodes[u].max_y = nodes[l].max_y;
    }
    if (nodes[u].right) {
        int r = nodes[u].right;
        if (nodes[r].min_x < nodes[u].min_x) nodes[u].min_x = nodes[r].min_x;
        if (nodes[r].max_x > nodes[u].max_x) nodes[u].max_x = nodes[r].max_x;
        if (nodes[r].min_y < nodes[u].min_y) nodes[u].min_y = nodes[r].min_y;
        if (nodes[r].max_y > nodes[u].max_y) nodes[u].max_y = nodes[r].max_y;
    }
}

// Build a perfectly balanced K-D Tree using max-spread split
int build(int l, int r, int parent) {
    if (l > r) return 0;
    
    int mid = l + (r - l) / 2;
    
    int min_x = cities[build_indices[l]].x, max_x = min_x;
    int min_y = cities[build_indices[l]].y, max_y = min_y;
    for (int i = l + 1; i <= r; ++i) {
        int idx = build_indices[i];
        if (cities[idx].x < min_x) min_x = cities[idx].x;
        else if (cities[idx].x > max_x) max_x = cities[idx].x;
        
        if (cities[idx].y < min_y) min_y = cities[idx].y;
        else if (cities[idx].y > max_y) max_y = cities[idx].y;
    }
    
    if (max_x - min_x >= max_y - min_y) {
        nth_element(build_indices + l, build_indices + mid, build_indices + r + 1, [](int a, int b) {
            return cities[a].x < cities[b].x;
        });
    } else {
        nth_element(build_indices + l, build_indices + mid, build_indices + r + 1, [](int a, int b) {
            return cities[a].y < cities[b].y;
        });
    }
    
    int u = build_indices[mid];
    nodes[u].x = cities[u].x;
    nodes[u].y = cities[u].y;
    nodes[u].parent = parent;
    nodes[u].dp = -INF;
    nodes[u].max_dp = -INF;

    nodes[u].left = build(l, mid - 1, u);
    nodes[u].right = build(mid + 1, r, u);

    pull_bbox(u);
    return u;
}

// 2D Range Maximum Query with Alpha-Beta style pruning
long long query(int u, int qx1, int qx2, int qy1, int qy2) {
    if (nodes[u].max_dp == -INF) return -INF;
    
    // If the node's entire bounding box is inside the query rectangle
    if (qx1 <= nodes[u].min_x && nodes[u].max_x <= qx2 &&
        qy1 <= nodes[u].min_y && nodes[u].max_y <= qy2) {
        return nodes[u].max_dp;
    }
    
    // If the node's entire bounding box is outside the query rectangle
    if (nodes[u].max_x < qx1 || nodes[u].min_x > qx2 ||
        nodes[u].max_y < qy1 || nodes[u].min_y > qy2) {
        return -INF;
    }

    long long res = -INF;
    // Check the specific point at this node
    if (qx1 <= nodes[u].x && nodes[u].x <= qx2 &&
        qy1 <= nodes[u].y && nodes[u].y <= qy2) {
        res = nodes[u].dp;
    }

    int l = nodes[u].left;
    int r = nodes[u].right;

    // Check the more promising child first to improve pruning
    if (l && r) {
        if (nodes[l].max_dp < nodes[r].max_dp) {
            swap(l, r);
        }
    }

    if (l && nodes[l].max_dp > res) {
        long long l_res = query(l, qx1, qx2, qy1, qy2);
        if (l_res > res) res = l_res;
    }
    if (r && nodes[r].max_dp > res) {
        long long r_res = query(r, qx1, qx2, qy1, qy2);
        if (r_res > res) res = r_res;
    }

    return res;
}

// Point Update via parent links
void update(int u, long long val) {
    nodes[u].dp = val;
    while (u) {
        long long old_max = nodes[u].max_dp;
        long long m = nodes[u].dp;
        
        if (nodes[u].left && nodes[nodes[u].left].max_dp > m) {
            m = nodes[nodes[u].left].max_dp;
        }
        if (nodes[u].right && nodes[nodes[u].right].max_dp > m) {
            m = nodes[nodes[u].right].max_dp;
        }
        
        nodes[u].max_dp = m;
        
        if (nodes[u].max_dp == old_max) break; // Crucial early stopping
        u = nodes[u].parent;
    }
}

int main() {
    // Maximize stream performance
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, d_lat, d_long;
    if (!(cin >> n >> d_lat >> d_long)) return 0;

    for (int i = 1; i <= n; ++i) {
        cin >> cities[i].x >> cities[i].y >> cities[i].h >> cities[i].p;
        build_indices[i] = i;
    }

    // Build K-D Tree topology
    int root = build(1, n, 0);

    // Create an array to process cities by height descending
    vector<int> process_indices(n);
    iota(process_indices.begin(), process_indices.end(), 1);
    
    // HackerRank C++ environment compatibility for sorting
    sort(process_indices.begin(), process_indices.end(), [](int a, int b) {
        return cities[a].h > cities[b].h;
    });

    long long overall_max = -INF;

    // Process cities. Group identical heights together to enforce strictly greater height
    for (int i = 0; i < n; ) {
        int j = i;
        while (j < n && cities[process_indices[j]].h == cities[process_indices[i]].h) {
            j++;
        }
        
        // 1. Calculate DP for all elements of the identical height group
        for (int k = i; k < j; ++k) {
            int u = process_indices[k];
            int qx1 = cities[u].x - d_lat;
            int qx2 = cities[u].x + d_lat;
            int qy1 = cities[u].y - d_long;
            int qy2 = cities[u].y + d_long;
            
            long long max_next = query(root, qx1, qx2, qy1, qy2);
            long long my_dp = cities[u].p;
            
            // Only jump if it yields a positive net gain over stopping here
            if (max_next > 0) {
                my_dp += max_next;
            }
            
            group_dp[k - i] = my_dp;
            if (my_dp > overall_max) {
                overall_max = my_dp;
            }
        }
        
        // 2. Commit the new DP states back into the K-D Tree
        for (int k = i; k < j; ++k) {
            int u = process_indices[k];
            update(u, group_dp[k - i]);
        }
        i = j;
    }

    cout << overall_max << "\n";
    return 0;
}
