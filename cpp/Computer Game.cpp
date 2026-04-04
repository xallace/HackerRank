#include <bits/stdc++.h>

using namespace std;

string ltrim(const string &);
string rtrim(const string &);
vector<string> split(const string &);

const int MAX_PRIME = 31623;
vector<int> primes;
bool is_prime[MAX_PRIME + 1];

// Precompute primes up to sqrt(10^9) to efficiently factorize the array elements
void sieve() {
    fill(is_prime, is_prime + MAX_PRIME + 1, true);
    is_prime[0] = is_prime[1] = false;
    for (int p = 2; p * p <= MAX_PRIME; p++) {
        if (is_prime[p]) {
            for (int i = p * p; i <= MAX_PRIME; i += p) {
                is_prime[i] = false;
            }
        }
    }
    for (int p = 2; p <= MAX_PRIME; p++) {
        if (is_prime[p]) primes.push_back(p);
    }
}

// Factorize a number down to its unique prime factors
vector<int> get_prime_factors(int x) {
    vector<int> factors;
    int temp = x;
    for (int p : primes) {
        if (p * 1ll * p > temp) break;
        if (temp % p == 0) {
            factors.push_back(p);
            do {
                temp /= p;
            } while (temp % p == 0);
        }
    }
    if (temp > 1) {
        factors.push_back(temp);
    }
    return factors;
}

struct Edge {
    int to;
    int cap;
    int flow;
    int next;
};

vector<int> head;
vector<Edge> edges;
vector<int> level_arr;
vector<int> ptr;
int num_nodes;

void add_edge(int from, int to, int cap) {
    edges.push_back({to, cap, 0, head[from]});
    head[from] = edges.size() - 1;
    
    // Reverse edge for residual graph
    edges.push_back({from, 0, 0, head[to]});
    head[to] = edges.size() - 1;
}

// BFS to construct the level graph for Dinic's Algorithm
bool bfs(int s, int t) {
    fill(level_arr.begin(), level_arr.end(), -1);
    level_arr[s] = 0;
    queue<int> q;
    q.push(s);
    while (!q.empty()) {
        int v = q.front();
        q.pop();
        for (int cid = head[v]; cid != -1; cid = edges[cid].next) {
            int tr = edges[cid].to;
            if (edges[cid].cap - edges[cid].flow > 0 && level_arr[tr] == -1) {
                level_arr[tr] = level_arr[v] + 1;
                q.push(tr);
            }
        }
    }
    return level_arr[t] != -1;
}

// DFS to push flow along augmenting paths
int dfs(int v, int t, int pushed) {
    if (pushed == 0) return 0;
    if (v == t) return pushed;
    for (int& cid = ptr[v]; cid != -1; cid = edges[cid].next) {
        int tr = edges[cid].to;
        if (level_arr[v] + 1 != level_arr[tr] || edges[cid].cap - edges[cid].flow == 0) continue;
        int push = dfs(tr, t, min(pushed, edges[cid].cap - edges[cid].flow));
        if (push == 0) continue;
        
        edges[cid].flow += push;
        edges[cid ^ 1].flow -= push;
        return push;
    }
    return 0;
}

int dinic(int s, int t) {
    int flow = 0;
    while (bfs(s, t)) {
        ptr = head;
        while (int pushed = dfs(s, t, 1e9)) {
            flow += pushed;
        }
    }
    return flow;
}

/*
 * Complete the 'computerGame' function below.
 *
 * The function is expected to return an INTEGER.
 * The function accepts following parameters:
 * 1. INTEGER_ARRAY a
 * 2. INTEGER_ARRAY b
 */
int computerGame(vector<int> a, vector<int> b) {
    if (primes.empty()) sieve();
    
    int n = a.size();
    vector<vector<int>> A_factors(n);
    vector<vector<int>> B_factors(n);
    vector<int> all_primes_used;
    all_primes_used.reserve(n * 10);
    
    // Break arrays down and register any encountered unique primes
    for (int i = 0; i < n; i++) {
        A_factors[i] = get_prime_factors(a[i]);
        for (int p : A_factors[i]) all_primes_used.push_back(p);
    }
    for (int i = 0; i < n; i++) {
        B_factors[i] = get_prime_factors(b[i]);
        for (int p : B_factors[i]) all_primes_used.push_back(p);
    }
    
    sort(all_primes_used.begin(), all_primes_used.end());
    all_primes_used.erase(unique(all_primes_used.begin(), all_primes_used.end()), all_primes_used.end());
    
    int num_primes = all_primes_used.size();
    
    // 1 Source + N for A + N for B + primes + 1 Sink
    num_nodes = 2 * n + num_primes + 2; 
    head.assign(num_nodes, -1);
    edges.clear();
    edges.reserve(5000000); 
    level_arr.resize(num_nodes);
    ptr.resize(num_nodes);
    
    int source = 0;
    int sink = num_nodes - 1;
    
    // Connect elements and their intermediate primes
    for (int i = 0; i < n; i++) {
        add_edge(source, i + 1, 1);
        for (int p : A_factors[i]) {
            int p_id = lower_bound(all_primes_used.begin(), all_primes_used.end(), p) - all_primes_used.begin();
            add_edge(i + 1, 2 * n + 1 + p_id, 1);
        }
    }
    
    for (int i = 0; i < n; i++) {
        for (int p : B_factors[i]) {
            int p_id = lower_bound(all_primes_used.begin(), all_primes_used.end(), p) - all_primes_used.begin();
            add_edge(2 * n + 1 + p_id, n + 1 + i, 1);
        }
        add_edge(n + 1 + i, sink, 1);
    }
    
    // Max flow naturally equals the max bipartite matching
    return dinic(source, sink);
}

int main() {
    ofstream fout(getenv("OUTPUT_PATH") ? getenv("OUTPUT_PATH") : "output.txt");

    string n_temp;
    if (!getline(cin, n_temp)) return 0;

    int n = stoi(ltrim(rtrim(n_temp)));

    string a_temp_temp;
    getline(cin, a_temp_temp);

    vector<string> a_temp = split(rtrim(a_temp_temp));

    vector<int> a(n);
    for (int i = 0; i < n; i++) {
        int a_item = stoi(a_temp[i]);
        a[i] = a_item;
    }

    string b_temp_temp;
    getline(cin, b_temp_temp);

    vector<string> b_temp = split(rtrim(b_temp_temp));

    vector<int> b(n);
    for (int i = 0; i < n; i++) {
        int b_item = stoi(b_temp[i]);
        b[i] = b_item;
    }

    int result = computerGame(a, b);

    fout << result << "\n";
    return 0;
}

// C++20 compliant string trims
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
