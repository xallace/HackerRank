// Complete, well-commented, runnable code for this single file
#include <iostream>
#include <algorithm>

using namespace std;

// Using a 128-bit unsigned integer type to automatically handle huge 
// quadratic multiplications (m * w) without worrying about overflow edge cases.
typedef unsigned __int128 u128;

int main() {
    // Optimize standard I/O operations for performance
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    unsigned long long m_in, w_in, p_in, n_in;
    if (!(cin >> m_in >> w_in >> p_in >> n_in)) return 0;

    u128 m = m_in;
    u128 w = w_in;
    u128 p = p_in;
    u128 n = n_in;

    u128 passes = 0;
    u128 candies = 0;
    u128 ans = n; // The absolute maximum passes we would need is n (if we produce 1/day).

    // If our initial production is enough to hit the target in one pass
    if (n <= m * w) {
        cout << 1 << "\n";
        return 0;
    }

    while (candies < n) {
        u128 current_production = m * w;
        
        // Calculate how many days it would take to reach 'n' if we NEVER bought anything else from this point.
        // We evaluate this "stop buying" strategy at every single step.
        u128 days_to_n = (n - candies + current_production - 1) / current_production;
        
        // Update the global minimal passes found so far
        if (passes + days_to_n < ans) {
            ans = passes + days_to_n;
        }

        // Calculate how many days we need to fast-forward until we can buy at least one upgrade.
        u128 days_to_p = 0;
        if (candies < p) {
            days_to_p = (p - candies + current_production - 1) / current_production;
        }

        // Fast-forward time to skip empty days
        passes += days_to_p;
        candies += days_to_p * current_production;

        // If the elapsed passes already exceed our known best answer, we can safely stop looking deeper.
        if (passes >= ans) {
            break;
        }

        // If we have enough candies to buy upgrades, greedily buy as many as possible.
        if (candies >= p) {
            u128 buy = candies / p;
            candies %= p; // Remaining candies after max purchase

            // To maximize the resulting production area (m * w), the dimensions should be 
            // as close to a square as possible. First, try to balance m and w.
            if (m > w) {
                u128 diff = m - w;
                if (buy < diff) {
                    w += buy;
                    buy = 0;
                } else {
                    w += diff;
                    buy -= diff;
                }
            } else if (w > m) {
                u128 diff = w - m;
                if (buy < diff) {
                    m += buy;
                    buy = 0;
                } else {
                    m += diff;
                    buy -= diff;
                }
            }

            // If they are balanced and we still have upgrades to apply, distribute them evenly.
            m += buy / 2;
            w += buy - buy / 2;
        }
    }

    // Output the minimum passes needed, downcast back to 64-bit integer
    cout << (unsigned long long)ans << "\n";
    return 0;
}
