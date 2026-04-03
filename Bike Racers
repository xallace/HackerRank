#include <bits/stdc++.h>
using namespace std;

// Hopcroft-Karp bipartite matching - O(E * sqrt(V))
struct HopcroftKarp {
    int n, m;
    vector<vector<int>> adj;
    vector<int> matchL, matchR, dist, cur;

    HopcroftKarp(int n, int m)
        : n(n), m(m), adj(n), matchL(n, -1), matchR(m, -1), dist(n), cur(n) {}

    void addEdge(int u, int v) { adj[u].push_back(v); }

    bool bfs() {
        queue<int> q;
        for (int u = 0; u < n; u++) {
            if (matchL[u] == -1) { dist[u] = 0; q.push(u); }
            else dist[u] = INT_MAX;
        }
        bool found = false;
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (int v : adj[u]) {
                int w = matchR[v];
                if (w == -1) {
                    found = true;          // free right node found - don't enqueue
                } else if (dist[w] == INT_MAX) {
                    dist[w] = dist[u] + 1;
                    q.push(w);
                }
            }
        }
        return found;
    }

    // DFS with current-edge pointer to avoid re-traversal
    bool dfs(int u) {
        for (int& i = cur[u]; i < (int)adj[u].size(); i++) {
            int v = adj[u][i];
            int w = matchR[v];
            if (w == -1 || (dist[w] == dist[u] + 1 && dfs(w))) {
                matchL[u] = v;
                matchR[v] = u;
                return true;
            }
        }
        dist[u] = INT_MAX;   // prune this node in future DFS calls this phase
        return false;
    }

    int maxMatching() {
        int res = 0;
        while (bfs()) {
            fill(cur.begin(), cur.end(), 0);
            for (int u = 0; u < n; u++)
                if (matchL[u] == -1)
                    res += dfs(u);
        }
        return res;
    }
};

// Safe integer square root: floor(sqrt(n))
long long isqrt(long long n) {
    long long r = (long long)sqrtl((long double)n);
    while (r > 0 && r * r > n) r--;
    while ((r + 1) * (r + 1) <= n) r++;
    return r;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, k;
    cin >> n >> m >> k;

    // n bikers, m bikes
    vector<long long> bx(n), by(n), cx(m), cy(m);
    for (int i = 0; i < n; i++) cin >> bx[i] >> by[i];
    for (int i = 0; i < m; i++) cin >> cx[i] >> cy[i];

    // Precompute all N*M squared distances
    auto sq_dist = [&](int i, int j) -> long long {
        long long dx = bx[i] - cx[j], dy = by[i] - cy[j];
        return dx * dx + dy * dy;
    };

    vector<long long> all_dists;
    all_dists.reserve((long long)n * m);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            all_dists.push_back(sq_dist(i, j));

    sort(all_dists.begin(), all_dists.end());
    all_dists.erase(unique(all_dists.begin(), all_dists.end()), all_dists.end());

    // Binary search over the sorted unique squared distances.
    // Find the smallest D^2 such that the bipartite matching has size >= k.
    int lo = 0, hi = (int)all_dists.size() - 1, ans_idx = hi;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        long long D2 = all_dists[mid];

        HopcroftKarp hk(n, m);
        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++)
                if (sq_dist(i, j) <= D2)
                    hk.addEdge(i, j);

        if (hk.maxMatching() >= k) {
            ans_idx = mid;
            hi = mid - 1;
        } else {
            lo = mid + 1;
        }
    }

    cout << all_dists[ans_idx] << "\n";
    return 0;
}
