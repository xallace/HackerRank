#include <bits/stdc++.h>

using namespace std;

string ltrim(const string &);
string rtrim(const string &);
vector<string> split(const string &);

/*
 * Complete the 'shortestReach' function below.
 *
 * The function is expected to return an INTEGER_ARRAY.
 * The function accepts following parameters:
 * 1. INTEGER n
 * 2. 2D_INTEGER_ARRAY edges
 * 3. INTEGER s
 */
vector<int> shortestReach(int n, vector<vector<int>> edges, int s) {
    // Adjacency list representation: adj[u] will hold pairs of {v, weight}
    vector<vector<pair<int, int>>> adj(n + 1);
    for (const auto& edge : edges) {
        int u = edge[0];
        int v = edge[1];
        int w = edge[2];
        adj[u].push_back({v, w});
        adj[v].push_back({u, w}); // The graph is undirected
    }
    
    // Distance array initialized to "infinity" to represent unreached nodes
    vector<int> dist(n + 1, INT_MAX);
    dist[s] = 0;
    
    // Min-heap to pick the node with the shortest known distance
    // Stores pairs of {distance, node}
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    pq.push({0, s});
    
    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();
        
        // If we found a strictly shorter path to u before popping this, 
        // this is a stale record in the priority queue. Ignore it.
        if (d > dist[u]) continue;
        
        // Explore all neighbors of node u
        for (const auto& neighbor : adj[u]) {
            int v = neighbor.first;
            int weight = neighbor.second;
            
            // Relaxation step
            if (dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                pq.push({dist[v], v});
            }
        }
    }
    
    // Collect the results for all nodes except the starting node 's'
    vector<int> result;
    for (int i = 1; i <= n; i++) {
        if (i != s) {
            // If the node remains at INT_MAX, it is unreachable
            result.push_back(dist[i] == INT_MAX ? -1 : dist[i]);
        }
    }
    
    return result;
}

int main() {
    ofstream fout(getenv("OUTPUT_PATH") ? getenv("OUTPUT_PATH") : "output.txt");

    string t_temp;
    if (!getline(cin, t_temp)) return 0;

    int t = stoi(ltrim(rtrim(t_temp)));

    for (int t_itr = 0; t_itr < t; t_itr++) {
        string first_multiple_input_temp;
        getline(cin, first_multiple_input_temp);

        vector<string> first_multiple_input = split(rtrim(first_multiple_input_temp));

        int n = stoi(first_multiple_input[0]);
        int m = stoi(first_multiple_input[1]);

        vector<vector<int>> edges(m);

        for (int i = 0; i < m; i++) {
            edges[i].resize(3);

            string edges_row_temp_temp;
            getline(cin, edges_row_temp_temp);

            vector<string> edges_row_temp = split(rtrim(edges_row_temp_temp));

            for (int j = 0; j < 3; j++) {
                int edges_row_item = stoi(edges_row_temp[j]);
                edges[i][j] = edges_row_item;
            }
        }

        string s_temp;
        getline(cin, s_temp);

        int s = stoi(ltrim(rtrim(s_temp)));

        vector<int> result = shortestReach(n, edges, s);

        for (size_t i = 0; i < result.size(); i++) {
            fout << result[i];

            if (i != result.size() - 1) {
                fout << " ";
            }
        }

        fout << "\n";
    }

    fout.close();
    return 0;
}

// C++20 compliant trims replacing deprecated std::not1
string ltrim(const string &str) {
    string s(str);
    s.erase(s.begin(), find_if(s.begin(), s.end(), [](unsigned char ch) { return !isspace(ch); }));
    return s;
}

string rtrim(const string &str) {
    string s(str);
    s.erase(find_if(s.rbegin(), s.rend(), [](unsigned char ch) { return !isspace(ch); }).base(), s.end());
    return s;
}

vector<string> split(const string &str) {
    vector<string> tokens;
    string::size_type start = 0;
    string::size_type end = 0;
    while ((end = str.find(" ", start)) != string::npos) {
        tokens.push_back(str.substr(start, end - start));
        start = end + 1;
    }
    tokens.push_back(str.substr(start));
    return tokens;
}
