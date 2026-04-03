#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <cmath>
#include <queue>

/**
 * Solve the Cut the Tree challenge.
 * Uses iterative BFS and bottom-up summation to avoid stack overflow.
 */
long long solveCutTheTree(const std::vector<int>& data, int n, const std::vector<std::pair<int, int>>& edges) {
    std::vector<std::vector<int>> adj(n);
    
    // Build adjacency list (1-indexed to 0-indexed conversion)
    for (const auto& edge : edges) {
        int u = edge.first - 1;
        int v = edge.second - 1;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    // Calculate total sum of all nodes using long long to avoid overflow
    long long total_sum = 0;
    for (int val : data) {
        total_sum += val;
    }

    std::vector<long long> subtree_sums(n, 0);
    std::vector<int> parent(n, -1);
    std::vector<int> order;
    order.reserve(n);
    
    // Iterative BFS to determine parent-child relationships and processing order
    std::vector<bool> visited(n, false);
    std::queue<int> q;
    
    q.push(0);
    visited[0] = true;
    
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        order.push_back(u);
        
        for (int v : adj[u]) {
            if (!visited[v]) {
                visited[v] = true;
                parent[v] = u;
                q.push(v);
            }
        }
    }

    // Calculate subtree sums bottom-up (from leaves to root)
    for (int i = n - 1; i >= 0; --i) {
        int u = order[i];
        subtree_sums[u] += data[u];
        if (parent[u] != -1) {
            subtree_sums[parent[u]] += subtree_sums[u];
        }
    }

    // Find the minimum difference
    // Removing an edge means separating a subtree from its parent.
    long long min_diff = -1;

    for (int i = 1; i < n; ++i) {
        long long s1 = subtree_sums[i];
        long long s2 = total_sum - s1;
        long long current_diff = std::abs(s1 - s2);
        
        if (min_diff == -1 || current_diff < min_diff) {
            min_diff = current_diff;
        }
    }

    return min_diff;
}

int main() {
    // Speed up I/O operations
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n;
    if (!(std::cin >> n)) return 0;

    std::vector<int> data(n);
    for (int i = 0; i < n; ++i) {
        std::cin >> data[i];
    }

    std::vector<std::pair<int, int>> edges(n - 1);
    for (int i = 0; i < n - 1; ++i) {
        std::cin >> edges[i].first >> edges[i].second;
    }

    std::cout << solveCutTheTree(data, n, edges) << std::endl;

    return 0;
}
