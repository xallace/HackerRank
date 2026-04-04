#include <bits/stdc++.h>

using namespace std;

string ltrim(const string &);
string rtrim(const string &);
vector<string> split(const string &);

// Segment Tree for lazy propagation of boolean values
struct SegmentTree {
    int n;
    vector<int> sum;
    vector<int> lazy;

    SegmentTree(int n) : n(n), sum(4 * n + 1, 0), lazy(4 * n + 1, -1) {}

    // Push the lazy tag to children
    inline void push(int node, int l, int r) {
        if (lazy[node] != -1) {
            int mid = l + (r - l) / 2;
            int left_child = 2 * node;
            int right_child = 2 * node + 1;
            
            lazy[left_child] = lazy[node];
            sum[left_child] = lazy[node] * (mid - l + 1);
            
            lazy[right_child] = lazy[node];
            sum[right_child] = lazy[node] * (r - mid);
            
            lazy[node] = -1;
        }
    }

    void build(int node, int l, int r, const vector<int>& a, int x) {
        lazy[node] = -1;
        if (l == r) {
            sum[node] = (a[l] >= x ? 1 : 0);
            return;
        }
        int mid = l + (r - l) / 2;
        build(2 * node, l, mid, a, x);
        build(2 * node + 1, mid + 1, r, a, x);
        sum[node] = sum[2 * node] + sum[2 * node + 1];
    }

    void update(int node, int l, int r, int ql, int qr, int val) {
        if (ql <= l && r <= qr) {
            lazy[node] = val;
            sum[node] = val * (r - l + 1);
            return;
        }
        push(node, l, r);
        int mid = l + (r - l) / 2;
        if (ql <= mid) update(2 * node, l, mid, ql, qr, val);
        if (qr > mid)  update(2 * node + 1, mid + 1, r, ql, qr, val);
        sum[node] = sum[2 * node] + sum[2 * node + 1];
    }

    int query(int node, int l, int r, int ql, int qr) {
        if (ql <= l && r <= qr) {
            return sum[node];
        }
        push(node, l, r);
        int mid = l + (r - l) / 2;
        int res = 0;
        if (ql <= mid) res += query(2 * node, l, mid, ql, qr);
        if (qr > mid)  res += query(2 * node + 1, mid + 1, r, ql, qr);
        return res;
    }
};

/*
 * Complete the 'sortedSubsegments' function below.
 *
 * The function is expected to return an INTEGER.
 * The function accepts following parameters:
 * 1. INTEGER k
 * 2. INTEGER_ARRAY a
 * 3. 2D_INTEGER_ARRAY queries
 */
int sortedSubsegments(int k, vector<int> a, vector<vector<int>> queries) {
    int n = a.size();
    if (n == 0) return 0;

    // Isolate unique values to binary search the answer against
    vector<int> sorted_a = a;
    sort(sorted_a.begin(), sorted_a.end());
    sorted_a.erase(unique(sorted_a.begin(), sorted_a.end()), sorted_a.end());

    SegmentTree st(n);

    int low = 0, high = (int)sorted_a.size() - 1;
    int ans = -1;

    // Binary search the optimal answer
    while (low <= high) {
        int mid = low + (high - low) / 2;
        int x = sorted_a[mid];

        // Treat element as 1 if a[i] >= x, else 0
        st.build(1, 0, n - 1, a, x);

        for (const auto& q : queries) {
            int l = q[0], r = q[1];
            if (l > r) swap(l, r);
            if (l < 0) l = 0;
            if (r >= n) r = n - 1;
            if (l >= r) continue;

            // Sort boolean segment via counting 1s
            int count1 = st.query(1, 0, n - 1, l, r);
            int count0 = (r - l + 1) - count1;

            if (count0 > 0) {
                st.update(1, 0, n - 1, l, l + count0 - 1, 0); // Put 0s at the start
            }
            if (count1 > 0) {
                st.update(1, 0, n - 1, l + count0, r, 1);     // Put 1s at the end
            }
        }

        // Check if the true value standing at index K is >= x
        if (st.query(1, 0, n - 1, k, k) == 1) {
            ans = x;
            low = mid + 1; // It's possible the true answer is larger
        } else {
            high = mid - 1; // True answer is strictly smaller than x
        }
    }

    return ans;
}

int main() {
    // HackerRank environment standard execution hook
    ofstream fout(getenv("OUTPUT_PATH") ? getenv("OUTPUT_PATH") : "output.txt");

    string first_multiple_input_temp;
    if (!getline(cin, first_multiple_input_temp)) return 0;

    vector<string> first_multiple_input = split(rtrim(first_multiple_input_temp));

    int n = stoi(first_multiple_input[0]);
    int q = stoi(first_multiple_input[1]);
    int k = stoi(first_multiple_input[2]);

    string a_temp_temp;
    getline(cin, a_temp_temp);

    vector<string> a_temp = split(rtrim(a_temp_temp));

    vector<int> a(n);
    for (int i = 0; i < n; i++) {
        int a_item = stoi(a_temp[i]);
        a[i] = a_item;
    }

    vector<vector<int>> queries(q);
    for (int i = 0; i < q; i++) {
        queries[i].resize(2);

        string queries_row_temp_temp;
        getline(cin, queries_row_temp_temp);

        vector<string> queries_row_temp = split(rtrim(queries_row_temp_temp));

        for (int j = 0; j < 2; j++) {
            int queries_row_item = stoi(queries_row_temp[j]);
            queries[i][j] = queries_row_item;
        }
    }

    int result = sortedSubsegments(k, a, queries);

    fout << result << "\n";
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
