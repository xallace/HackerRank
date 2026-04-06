#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <cmath>
#include <queue>

using namespace std;

struct Cell {
    int r, c;
};

int main() {
    int N, M, K;
    if (!(cin >> N >> M >> K)) return 0;

    vector<string> grid(N);
    for (int i = 0; i < N; ++i) {
        cin >> grid[i];
    }

    Cell tunnel_dest[21][21];
    for (int i = 0; i < 21; ++i) {
        for (int j = 0; j < 21; ++j) {
            tunnel_dest[i][j] = {-1, -1};
        }
    }

    for (int i = 0; i < K; ++i) {
        int r1, c1, r2, c2;
        cin >> r1 >> c1 >> r2 >> c2;
        r1--; c1--; r2--; c2--; // 0-indexed
        tunnel_dest[r1][c1] = {r2, c2};
        tunnel_dest[r2][c2] = {r1, c1};
    }

    int start_r = -1, start_c = -1;
    vector<Cell> states;
    int id[21][21];
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            id[i][j] = -1;
            if (grid[i][j] == 'A' || grid[i][j] == 'S') {
                start_r = i;
                start_c = j;
            }
            if (grid[i][j] == 'O' || grid[i][j] == 'A' || grid[i][j] == 'S' || tunnel_dest[i][j].r != -1) {
                // Potential state
                id[i][j] = states.size();
                states.push_back({i, j});
            }
        }
    }

    int V = states.size();
    vector<vector<double>> transitions(V, vector<double>(V, 0.0));
    vector<double> win_prob(V, 0.0);
    vector<bool> can_move(V, false);

    int dr[] = {-1, 1, 0, 0};
    int dc[] = {0, 0, -1, 1};

    for (int i = 0; i < V; ++i) {
        int r = states[i].r;
        int c = states[i].c;
        
        vector<Cell> neighbors;
        for (int d = 0; d < 4; ++d) {
            int nr = r + dr[d];
            int nc = c + dc[d];
            if (nr >= 0 && nr < N && nc >= 0 && nc < M && grid[nr][nc] != '#') {
                neighbors.push_back({nr, nc});
            }
        }

        int k = neighbors.size();
        if (k == 0) continue;
        can_move[i] = true;

        for (Cell next : neighbors) {
            int nr = next.r;
            int nc = next.c;

            // Apply teleportation if any
            if (tunnel_dest[nr][nc].r != -1) {
                int tr = tunnel_dest[nr][nc].r;
                int tc = tunnel_dest[nr][nc].c;
                nr = tr;
                nc = tc;
            }

            if (grid[nr][nc] == '%') {
                win_prob[i] += 1.0 / k;
            } else if (grid[nr][nc] == '*') {
                // Loss
            } else if (id[nr][nc] != -1) {
                transitions[i][id[nr][nc]] += 1.0 / k;
            } else {
                // Stuck or invalid? Usually O/A/W are the only others.
            }
        }
    }

    // Reachability to exit
    vector<bool> reaches_win(V, false);
    queue<int> q;
    // Actually, a cell "wins" if it has win_prob > 0 or can reach a cell that wins
    for (int i = 0; i < V; ++i) {
        if (win_prob[i] > 1e-12) {
            reaches_win[i] = true;
            q.push(i);
        }
    }

    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v = 0; v < V; ++v) {
            if (!reaches_win[v] && transitions[v][u] > 1e-12) {
                reaches_win[v] = true;
                q.push(v);
            }
        }
    }

    if (start_r == -1 || id[start_r][start_c] == -1 || !reaches_win[id[start_r][start_c]]) {
        cout << fixed << setprecision(10) << 0.0 << endl;
        return 0;
    }

    // Solve system for states that can reach win
    vector<int> active_states;
    vector<int> rev_map(V, -1);
    for (int i = 0; i < V; ++i) {
        if (reaches_win[i]) {
            rev_map[i] = active_states.size();
            active_states.push_back(i);
        }
    }

    int n = active_states.size();
    vector<vector<double>> mat(n, vector<double>(n + 1, 0.0));
    for (int i = 0; i < n; ++i) {
        int u = active_states[i];
        mat[i][i] = 1.0;
        for (int v_idx = 0; v_idx < V; ++v_idx) {
            if (transitions[u][v_idx] > 0) {
                if (reaches_win[v_idx]) {
                    mat[i][rev_map[v_idx]] -= transitions[u][v_idx];
                } else {
                    // This state leads to a non-winning sink, effectively a loss
                }
            }
        }
        mat[i][n] = win_prob[u];
    }

    // Gaussian Elimination
    for (int i = 0; i < n; i++) {
        int pivot = i;
        for (int j = i + 1; j < n; j++) {
            if (abs(mat[j][i]) > abs(mat[pivot][i])) pivot = j;
        }
        swap(mat[i], mat[pivot]);
        
        if (abs(mat[i][i]) < 1e-18) continue;
        
        for (int j = 0; j < n; j++) {
            if (i != j) {
                double factor = mat[j][i] / mat[i][i];
                for (int k = i; k <= n; k++) {
                    mat[j][k] -= factor * mat[i][k];
                }
            }
        }
    }

    double result = 0;
    int start_id = id[start_r][start_c];
    if (rev_map[start_id] != -1) {
        int idx = rev_map[start_id];
        if (abs(mat[idx][idx]) > 1e-18) {
            result = mat[idx][n] / mat[idx][idx];
        }
    }

    cout << fixed << setprecision(10) << result << endl;

    return 0;
}
