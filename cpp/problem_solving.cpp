#include <iostream>
#include <vector>
#include <cmath>
#include <queue>

using namespace std;

/*
 * Complete the 'problemSolving' function below.
 *
 * The function is expected to return an INTEGER.
 * The function accepts following parameters:
 * 1. INTEGER k
 * 2. INTEGER_ARRAY v
 */
int problemSolving(int k, vector<int> v) {
    int n = v.size();
    
    // adj[u] will store the bipartite graph edges from left side to right side
    // We 1-index the vertices for the Hopcroft-Karp algorithm
    vector<vector<int>> adj(n + 1);
    
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            // If the absolute difference is at least k, we can solve problem j after problem i
            if (abs(v[i] - v[j]) >= k) {
                adj[i + 1].push_back(j + 1);
            }
        }
    }

    // Hopcroft-Karp Algorithm to find the Maximum Bipartite Matching
    vector<int> pairU(n + 1, 0), pairV(n + 1, 0), dist(n + 1, 0);
    const int INF = 1e9;

    auto bfs = [&]() {
        queue<int> q;
        for (int u = 1; u <= n; ++u) {
            if (pairU[u] == 0) {
                dist[u] = 0;
                q.push(u);
            } else {
                dist[u] = INF;
            }
        }
        dist[0] = INF;

        while (!q.empty()) {
            int u = q.front();
            q.pop();

            if (dist[u] < dist[0]) {
                for (int v_node : adj[u]) {
                    if (dist[pairV[v_node]] == INF) {
                        dist[pairV[v_node]] = dist[u] + 1;
                        q.push(pairV[v_node]);
                    }
                }
            }
        }
        return dist[0] != INF;
    };

    // Using auto& self for C++14/20 lambda recursion
    auto dfs = [&](auto& self, int u) -> bool {
        if (u != 0) {
            for (int v_node : adj[u]) {
                if (dist[pairV[v_node]] == dist[u] + 1) {
                    if (self(self, pairV[v_node])) {
                        pairV[v_node] = u;
                        pairU[u] = v_node;
                        return true;
                    }
                }
            }
            dist[u] = INF;
            return false;
        }
        return true;
    };

    int matching = 0;
    while (bfs()) {
        for (int u = 1; u <= n; ++u) {
            if (pairU[u] == 0) {
                if (dfs(dfs, u)) {
                    matching++;
                }
            }
        }
    }

    // Minimum path cover = Total number of vertices - Maximum Bipartite Matching
    return n - matching;
}

// Minimal main function for local testing (HackerRank provides its own driver code)
int main() {
    int t;
    if (cin >> t) {
        while (t--) {
            int n, k;
            cin >> n >> k;
            vector<int> v(n);
            for (int i = 0; i < n; ++i) {
                cin >> v[i];
            }
            cout << problemSolving(k, v) << "\n";
        }
    }
    return 0;
}
