#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

/**
 * Matrix Land Solution
 * Complexity: O(N * M)
 * * We use Dynamic Programming to track the maximum score reaching each cell.
 * For each row, we calculate:
 * 1. msl/msr: Max subsegment sums ending/starting at each cell (within the row).
 * 2. ml/mr: Max extension sums to the left/right of each cell.
 * 3. L/R: Max score arriving at cell j from any cell to its left/right in the row above.
 */

const long long INF = 1e17;

int main() {
    // Fast I/O is required for large data sets
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, m;
    if (!(cin >> n >> m)) return 0;

    // dp[j] stores the maximum score reaching cell (i, j) ready to move down.
    // For the first row, we can start at any cell, effectively having 0 score before.
    vector<long long> dp(m, 0);

    for (int i = 0; i < n; ++i) {
        vector<long long> row(m);
        for (int j = 0; j < m; ++j) {
            cin >> row[j];
        }

        // 1. Calculate max subarray sums within the current row.
        // msl[j]: max sum of a subsegment ending at j.
        // msr[j]: max sum of a subsegment starting at j.
        vector<long long> msl(m), msr(m);
        msl[0] = row[0];
        for (int j = 1; j < m; ++j) msl[j] = max(row[j], row[j] + msl[j - 1]);
        
        msr[m - 1] = row[m - 1];
        for (int j = m - 2; j >= 0; --j) msr[j] = max(row[j], row[j] + msr[j + 1]);

        // ml[j] and mr[j] are the max non-negative extensions available from cell j.
        vector<long long> ml(m, 0), mr(m, 0);
        for (int j = 1; j < m; ++j) ml[j] = max(0LL, msl[j - 1]);
        for (int j = 0; j < m - 1; ++j) mr[j] = max(0LL, msr[j + 1]);

        // 2. Prefix sums for easy interval sum calculation.
        vector<long long> S(m);
        S[0] = row[0];
        for (int j = 1; j < m; ++j) S[j] = S[j - 1] + row[j];

        // 3. Calculate transitions for the current row.
        // L[j]: Max score if we enter row i at some k <= j and exit at j.
        // R[j]: Max score if we enter row i at some k >= j and exit at j.
        vector<long long> L(m), R(m);

        // Calculate L[j] using a prefix max.
        // Score = dp[k] + ml[k] + Sum(k, j) + mr[j]
        //       = (dp[k] + ml[k] - S[k-1]) + S[j] + mr[j]
        long long best_v = -INF;
        for (int j = 0; j < m; ++j) {
            long long current_k_val = dp[j] + ml[j] - (j > 0 ? S[j - 1] : 0);
            best_v = max(best_v, current_k_val);
            L[j] = best_v + S[j] + mr[j];
        }

        // Calculate R[j] using a suffix max.
        // Score = dp[k] + mr[k] + Sum(j, k) + ml[j]
        //       = (dp[k] + mr[k] + S[k]) - S[j-1] + ml[j]
        long long best_w = -INF;
        for (int j = m - 1; j >= 0; --j) {
            long long current_k_val = dp[j] + mr[j] + S[j];
            best_w = max(best_w, current_k_val);
            R[j] = best_w - (j > 0 ? S[j - 1] : 0) + ml[j];
        }

        // Update DP for the next row.
        for (int j = 0; j < m; ++j) {
            dp[j] = max(L[j], R[j]);
        }
    }

    // The result is the maximum value in the final DP row.
    long long ans = -INF;
    for (int j = 0; j < m; ++j) {
        if (dp[j] > ans) ans = dp[j];
    }
    cout << ans << endl;

    return 0;
}
