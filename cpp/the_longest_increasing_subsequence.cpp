#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

/**
 * Problem: Longest Increasing Subsequence (LIS)
 * Zeitkomplexitaet: O(N log N)
 * Platzkomplexitaet: O(N)
 * * Wir verwenden ein Hilfs-Array 'tails', wobei tails[i] das kleinste 
 * Endelement einer steigenden Teilfolge der Laenge i+1 speichert.
 */

int main() {
    // Schnelle Ein-/Ausgabe
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    if (!(cin >> n)) return 0;

    // 'tails' speichert die kleinsten Endelemente fuer jede Laenge.
    // Die Laenge von 'tails' entspricht am Ende der LIS-Laenge.
    vector<int> tails;
    tails.reserve(n);

    for (int i = 0; i < n; ++i) {
        int x;
        cin >> x;

        if (tails.empty() || x > tails.back()) {
            // Wenn x groesser ist als das letzte Element, verlaengert es die LIS.
            tails.push_back(x);
        } else {
            // Finde das erste Element in 'tails', das >= x ist, 
            // und ersetze es durch x (Binaere Suche).
            // Dies macht die Endelemente so klein wie moeglich, 
            // um Platz fuer zukuenftige Zahlen zu schaffen.
            auto it = lower_bound(tails.begin(), tails.end(), x);
            *it = x;
        }
    }

    // Die Anzahl der Elemente in 'tails' ist die Laenge der LIS.
    cout << tails.size() << endl;

    return 0;
}
