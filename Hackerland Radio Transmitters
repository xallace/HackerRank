#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    // Fast I/O: Disables synchronization between C and C++ standard streams
    // for maximum read/write performance.
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, k;
    if (!(cin >> n >> k)) return 0;

    vector<int> x(n);
    for (int i = 0; i < n; ++i) {
        cin >> x[i];
    }

    // HackerRank defaults to C++14/17. Using classic sort to ensure compilation:
    sort(x.begin(), x.end());

    int transmitters = 0;
    int i = 0;

    while (i < n) {
        transmitters++;
        
        // 1. Identify the leftmost uncovered house
        int loc = x[i];
        
        // 2. Move to the right as far as possible within range 'k' to place the transmitter.
        // We want to cover 'loc', so the furthest we can place it is at 'loc + k'.
        while (i < n && x[i] <= loc + k) {
            i++;
        }
        
        // The transmitter is placed at the last valid house we passed
        int center = x[i - 1];
        
        // 3. Move past all houses that are covered by the right-side range of the transmitter.
        while (i < n && x[i] <= center + k) {
            i++;
        }
    }

    cout << transmitters << "\n";
    return 0;
}
