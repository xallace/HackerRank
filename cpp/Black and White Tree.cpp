#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <cmath>
#include <map>
#include <bitset>
#include <queue>

using namespace std;

/**
 * Problem: Black and White Tree
 * Logic:
 * 1. The graph consists of several connected components. Since the original graph 
 * was bipartite, every component is bipartite.
 * 2. Find the two partitions (sizes w_i, b_i) for each component.
 * 3. We want to choose one partition from each component to be 'White' such that 
 * the total White count W is as close to N/2 as possible.
 * 4. This is a variation of the Subset Sum problem. With N=10^5, we use a bitset 
 * and binary decomposition of counts for efficiency.
 * 5. Finally, connect the components by adding edges between opposite colors 
 * to keep the graph bipartite.
 */

struct Component {
    vector<int> p0;
    vector<int> p1;
};

void solve() {
    int n, m;
    if (!(cin >> n >> m)) return;

    if (n == 1) {
        cout << "0 0" << endl;
        return;
    }

    vector<vector<int>> adj(n + 1);
    for (int i = 0; i < m; ++i) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    vector<int> color(n + 1, -1);
    vector<int> node_to_comp(n + 1, -1);
    vector<Component> components;

    for (int i = 1; i <= n; ++i) {
        if (color[i] == -1) {
            int comp_idx = components.size();
            Component comp;
            queue<pair<int, int>> q;
            q.push({i, 0});
            color[i] = 0;

            while (!q.empty()) {
                auto [u, c] = q.front();
                q.pop();

                node_to_comp[u] = comp_idx;
                if (c == 0) comp.p0.push_back(u);
                else comp.p1.push_back(u);

                for (int v : adj[u]) {
                    if (color[v] == -1) {
                        color[v] = 1 - c;
                        q.push({v, 1 - c});
                    }
                }
            }
            components.push_back(comp);
        }
    }

    int base_w = 0;
    vector<int> diffs;
    for (auto& comp : components) {
        int s0 = comp.p0.size();
        int s1 = comp.p1.size();
        if (s0 > s1) swap(comp.p0, comp.p1);
        base_w += comp.p0.size();
        diffs.push_back(comp.p1.size() - comp.p0.size());
    }

    map<int, int> diff_counts;
    for (int d : diffs) {
        if (d > 0) diff_counts[d]++;
    }

    // Bitset DP with binary decomposition for Subset Sum
    // We use a fixed size bitset for max N = 200,000
    static bitset<200001> dp;
    vector<pair<int, bitset<200001>>> history;
    dp[0] = 1;

    for (auto const& [d, count] : diff_counts) {
        history.push_back({d, dp});
        int remaining = count;
        for (int p = 1; p <= remaining; p <<= 1) {
            dp |= (dp << (p * d));
            remaining -= p;
        }
        if (remaining > 0) {
            dp |= (dp << (remaining * d));
        }
    }

    int best_s = 0;
    int min_final_diff = n + 1;

    for (int s = 0; s <= n; ++s) {
        if (dp[s]) {
            int current_w = base_w + s;
            int current_abs_diff = abs(2 * current_w - n);
            if (current_abs_diff < min_final_diff) {
                min_final_diff = current_abs_diff;
                best_s = s;
            }
        }
    }

    // Backtrack to find which partitions to flip
    int curr_s = best_s;
    reverse(history.begin(), history.end());

    vector<bool> should_flip(components.size(), false);
    vector<vector<int>> diff_to_indices(n + 1);
    for(int i=0; i < (int)components.size(); ++i) {
        int d = components[i].p1.size() - components[i].p0.size();
        if (d > 0) diff_to_indices[d].push_back(i);
    }

    for (auto& hist : history) {
        int d = hist.first;
        bitset<200001>& prev_dp = hist.second;
        
        int count = diff_counts[d];
        for (int k = 0; k <= count; ++k) {
            int prev_s = curr_s - k * d;
            if (prev_s >= 0 && prev_dp[prev_s]) {
                for (int j = 0; j < k; ++j) {
                    int idx = diff_to_indices[d].back();
                    diff_to_indices[d].pop_back();
                    should_flip[idx] = true;
                }
                curr_s = prev_s;
                break;
            }
        }
    }

    struct Group { vector<int> w, b; };
    vector<Group> groups(components.size());
    vector<int> global_w, global_b;

    for (int i = 0; i < (int)components.size(); ++i) {
        if (should_flip[i]) {
            for (int u : components[i].p1) { groups[i].w.push_back(u); global_w.push_back(u); }
            for (int u : components[i].p0) { groups[i].b.push_back(u); global_b.push_back(u); }
        } else {
            for (int u : components[i].p0) { groups[i].w.push_back(u); global_w.push_back(u); }
            for (int u : components[i].p1) { groups[i].b.push_back(u); global_b.push_back(u); }
        }
    }

    // Connect components
    int anchor_w = global_w[0];
    int anchor_b = global_b[0];
    int comp_w_idx = node_to_comp[anchor_w];
    int comp_b_idx = node_to_comp[anchor_b];

    vector<pair<int, int>> new_edges;
    vector<bool> visited_comp(components.size(), false);
    visited_comp[comp_w_idx] = true;

    if (comp_w_idx != comp_b_idx) {
        new_edges.push_back({anchor_w, anchor_b});
        visited_comp[comp_b_idx] = true;
    }

    for (int i = 0; i < (int)components.size(); ++i) {
        if (visited_comp[i]) continue;
        
        // Maintain bipartite property: connect White to Black or Black to White
        if (!groups[i].b.empty()) {
            new_edges.push_back({anchor_w, groups[i].b[0]});
        } else {
            new_edges.push_back({anchor_b, groups[i].w[0]});
        }
        visited_comp[i] = true;
    }

    cout << min_final_diff << " " << new_edges.size() << "\n";
    for (auto& edge : new_edges) {
        cout << edge.first << " " << edge.second << "\n";
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    solve();
    return 0;
}
