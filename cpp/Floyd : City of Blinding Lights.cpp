#include <iostream>
#include <vector>
#include <map>
// #include <ranges> // Removed due to compiler incompatibility

using namespace std;

vector<int> floyd(int road_nodes, int road_edges, 
                  vector<int> road_from, vector<int> road_to, vector<int> road_weight, 
                  vector<int> x, vector<int> y) {
    
    // Convert arrays to 0-based indexing
    for (int& i : road_from) i--;
    for (int& i : road_to) i--;
    for (int& i : x) i--;
    for (int& i : y) i--;

    const int inf = 139651;

    vector<vector<int>> dist(road_nodes, vector<int>(road_nodes, inf));
    vector<map<int, int>> connected_to(road_nodes);

    for (int i = 0; i < road_nodes; ++i) {
        dist[i][i] = 0;
    }

    for (int i = 0; i < road_edges; ++i) {
        int u = road_from[i];
        int v = road_to[i];
        int w = road_weight[i];
        dist[u][v] = w;
        connected_to[u][v] = w;
    }

    // Iterative relaxation mimicking the Python code
    for (int i = 0; i < road_nodes; ++i) {
        for (int step = 0; step < road_nodes; ++step) {
            bool done = true;
            for (int k = 0; k < road_nodes; ++k) {
                // Using standard iterator instead of std::views::keys
                for (auto const& [j, weight] : connected_to[k]) {
                    if (dist[i][j] > dist[i][k] + dist[k][j]) {
                        dist[i][j] = dist[i][k] + dist[k][j];
                        done = false;
                    }
                }
            }
            if (done) {
                break;
            }
        }
    }

    vector<int> result;
    result.reserve(x.size());
    for (size_t i = 0; i < x.size(); ++i) {
        int u = x[i];
        int v = y[i];
        if (dist[u][v] < inf) {
            result.push_back(dist[u][v]);
        } else {
            result.push_back(-1);
        }
    }

    return result;
}

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int road_nodes, road_edges;
    if (!(cin >> road_nodes >> road_edges)) return 0;

    vector<int> road_from(road_edges), road_to(road_edges), road_weight(road_edges);
    for (int i = 0; i < road_edges; ++i) {
        cin >> road_from[i] >> road_to[i] >> road_weight[i];
    }

    int q;
    if (!(cin >> q)) return 0;

    vector<int> x(q), y(q);
    for (int i = 0; i < q; ++i) {
        cin >> x[i] >> y[i];
    }

    vector<int> z = floyd(road_nodes, road_edges, road_from, road_to, road_weight, x, y);
    
    for (int val : z) {
        cout << val << "\n";
    }

    return 0;
}
