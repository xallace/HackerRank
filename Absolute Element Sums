#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

/**
 * Problem: Playing with Numbers (HackerRank)
 * * Logic:
 * 1. The updates are cumulative. Let 'cumulative_delta' be the sum of all x values 
 * received in queries so far. After a query, the value of an original element 
 * A[i] becomes A[i] + cumulative_delta.
 * 2. We need to calculate sum(|A[i] + cumulative_delta|) for all i.
 * 3. By sorting A, we can find a split point 'idx' using binary search such that:
 * - For j < idx: A[j] + cumulative_delta < 0  => |A[j] + delta| = -(A[j] + delta)
 * - For j >= idx: A[j] + cumulative_delta >= 0 => |A[j] + delta| = (A[j] + delta)
 * 4. Sum = -[sum(A[0...idx-1]) + idx * delta] + [sum(A[idx...n-1]) + (n - idx) * delta]
 * 5. Prefix sums allow us to calculate sum(A[0...idx-1]) and sum(A[idx...n-1]) in O(1).
 */

using namespace std;

int main() {
    // Speed up I/O for large constraints
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    if (!(cin >> n)) return 0;

    vector<long long> a(n);
    for (int i = 0; i < n; ++i) {
        cin >> a[i];
    }

    // Sort the array to enable binary search and prefix sum logic
    sort(a.begin(), a.end());

    // Build prefix sums: prefix[i] = sum of a[0...i-1]
    vector<long long> prefix(n + 1, 0);
    for (int i = 0; i < n; ++i) {
        prefix[i + 1] = prefix[i] + a[i];
    }

    int q;
    cin >> q;

    long long cumulative_delta = 0;
    while (q--) {
        long long x;
        cin >> x;
        cumulative_delta += x;

        // Find the index where A[j] + cumulative_delta >= 0 
        // This is equivalent to A[j] >= -cumulative_delta
        auto it = lower_bound(a.begin(), a.end(), -cumulative_delta);
        int idx = distance(a.begin(), it);

        // Elements from [0, idx-1] are negative after adding delta
        long long sum_neg_raw = prefix[idx];
        long long count_neg = idx;
        long long total_neg = -(sum_neg_raw + count_neg * cumulative_delta);

        // Elements from [idx, n-1] are non-negative after adding delta
        long long sum_pos_raw = prefix[n] - prefix[idx];
        long long count_pos = n - idx;
        long long total_pos = sum_pos_raw + count_pos * cumulative_delta;

        cout << total_neg + total_pos << "\n";
    }

    return 0;
}
