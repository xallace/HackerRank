#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

int main() {
    // Fast I/O for maximum performance
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    long long n, m;
    int k;
    if (!(cin >> n >> m >> k)) return 0;

    // Map row number to a vector of column intervals [c1, c2]
    map<long long, vector<pair<long long, long long>>> tracks;

    for (int i = 0; i < k; ++i) {
        long long r, c1, c2;
        cin >> r >> c1 >> c2;
        tracks[r].push_back({c1, c2});
    }

    long long occupied = 0;

    // Process each row that has train tracks
    for (auto& entry : tracks) {
        auto& intervals = entry.second;
        
        // Sort intervals by their starting column to easily merge them left-to-right
        sort(intervals.begin(), intervals.end());

        long long current_start = intervals[0].first;
        long long current_end = intervals[0].second;

        for (size_t i = 1; i < intervals.size(); ++i) {
            if (intervals[i].first <= current_end) {
                // If intervals overlap or touch, extend the current tracked block
                current_end = max(current_end, intervals[i].second);
            } else {
                // No overlap: add the size of the tracked block and start a new one
                occupied += (current_end - current_start + 1);
                current_start = intervals[i].first;
                current_end = intervals[i].second;
            }
        }
        // Don't forget to add the final block in the row!
        occupied += (current_end - current_start + 1);
    }

    // Calculate total empty cells
    // Note: n * m can be up to 10^18, which fits perfectly in a 64-bit 'long long'
    long long total_cells = n * m;
    cout << total_cells - occupied << "\n";

    return 0;
}
