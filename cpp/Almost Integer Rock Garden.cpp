#include <iostream>
#include <vector>
#include <cmath>
#include <map>

using namespace std;

struct Point {
    int x, y;
};

int main() {
    // Fast I/O for maximum performance
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int x_in, y_in;
    if (!(cin >> x_in >> y_in)) return 0;

    // m2 will map distance^2 to a list of valid coordinates.
    // std::map automatically sorts by keys, matching Java's TreeMap behavior perfectly.
    map<int, vector<Point>> m2;

    // Precompute all valid non-integer distance points
    for (int i = -12; i <= 12; i++) {
        for (int j = -12; j <= 12; j++) {
            int d2 = i * i + j * j;
            if (d2 == 0) continue; // Distance 0 is an integer
            
            // Fast integer perfect square check (replaces floating point isZero check)
            int r = round(sqrt(d2));
            if (r * r == d2) continue; 
            
            m2[d2].push_back({i, j});
        }
    }

    int targetD2 = x_in * x_in + y_in * y_in;
    int targetidx = -1;
    
    vector<int> vs;
    int idx = 0;
    
    // Map the unique distances to an index array
    for (const auto& entry : m2) {
        vs.push_back(entry.first);
        if (entry.first == targetD2) {
            targetidx = idx;
        }
        idx++;
    }

    // Precalculated valid groups that sum to an almost integer
    int groups[22][12] = {
        {3, 14, 14, 16, 31, 31, 31, 45, 46, 50, 52, 52},
        {3, 4, 13, 19, 32, 33, 41, 43, 47, 59, 59, 59},
        {21, 24, 36, 41, 41, 44, 47, 54, 56, 58, 58, 62},
        {0, 9, 31, 31, 33, 34, 34, 37, 53, 56, 56, 63},
        {9, 15, 15, 31, 31, 33, 34, 34, 37, 56, 60, 63},
        {2, 3, 6, 8, 8, 8, 20, 25, 39, 43, 56, 62},
        {0, 24, 27, 29, 40, 40, 48, 49, 49, 54, 55, 55},
        {0, 2, 2, 3, 8, 20, 25, 34, 39, 43, 56, 62},
        {21, 24, 26, 36, 44, 47, 54, 56, 56, 58, 58, 62},
        {9, 26, 26, 26, 31, 31, 33, 34, 34, 37, 60, 63},
        {11, 12, 29, 31, 35, 37, 43, 44, 44, 57, 60, 64},
        {11, 12, 29, 31, 35, 37, 43, 44, 44, 53, 57, 67},
        {9, 10, 21, 21, 24, 25, 30, 38, 41, 44, 48, 62},
        {2, 4, 16, 19, 30, 33, 41, 43, 47, 59, 59, 59},
        {4, 8, 13, 25, 27, 28, 37, 43, 45, 49, 51, 65},
        {7, 9, 31, 31, 33, 34, 34, 37, 41, 41, 60, 63},
        {18, 18, 28, 30, 31, 40, 42, 44, 48, 49, 61, 66},
        {1, 23, 27, 28, 30, 31, 33, 46, 54, 61, 66, 66},
        {3, 5, 16, 22, 31, 31, 31, 45, 46, 52, 52, 54},
        {0, 10, 13, 30, 31, 31, 31, 45, 46, 50, 52, 54},
        {3, 4, 4, 4, 8, 16, 20, 25, 34, 43, 56, 62},
        {3, 4, 4, 8, 16, 17, 20, 25, 34, 39, 43, 56}
    };

    vector<int> g;
    // Find the matching target index group
    for (int i = 0; i < 22; i++) {
        for (int j = 0; j < 12; j++) {
            if (groups[i][j] == targetidx) {
                g.assign(groups[i], groups[i] + 12);
                break;
            }
        }
        if (!g.empty()) break;
    }

    // used_idx replicates the behavior of removeFirst() optimally without memory shifting
    map<int, int> used_idx;
    bool first_target_skipped = false;

    // Output strictly 11 points
    for (int i = 0; i < 12; i++) {
        int current_idx = g[i];
        
        // Skip the first occurrence of the input distance
        if (current_idx == targetidx && !first_target_skipped) {
            first_target_skipped = true;
            continue;
        }

        int d2_val = vs[current_idx];
        Point pt = m2[d2_val][used_idx[d2_val]++];
        
        // Ensure we don't accidentally output the user's starting stone 
        if (pt.x == x_in && pt.y == y_in) {
            pt = m2[d2_val][used_idx[d2_val]++];
        }

        cout << pt.x << " " << pt.y << "\n";
    }

    return 0;
}
