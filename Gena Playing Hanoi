#include <iostream>
#include <vector>
#include <queue>
#include <array>

using namespace std;

/**
 * Loesung fuer das Gena Playing Hanoi Problem.
 * Da N maximal 10 ist, gibt es 4^10 (ca. 1 Million) Zustaende.
 * Dies erlaubt eine Breitensuche (BFS), um die minimale Anzahl an Zuegen zu finden.
 */
int solve() {
    int n;
    if (!(cin >> n)) return -1;

    // Wir speichern den Ort jeder Scheibe in 2 Bits (0-3 fuer Stab 1-4).
    // Scheibe 0 ist die kleinste, Scheibe n-1 die groesste.
    int start_state = 0;
    for (int i = 0; i < n; ++i) {
        int rod;
        cin >> rod;
        rod--; // Umwandlung in 0-basierten Index (0, 1, 2, 3)
        start_state |= (rod << (2 * i));
    }

    // Ziel: Alle Scheiben auf Stab 1 (Index 0).
    int target_state = 0;
    if (start_state == target_state) return 0;

    // BFS Strukturen
    // 1 << 20 entspricht 4^10 Zustaenden.
    const int MAX_STATES = 1 << (2 * 10);
    vector<int> dist(MAX_STATES, -1);
    queue<int> q;

    dist[start_state] = 0;
    q.push(start_state);

    while (!q.empty()) {
        int current_state = q.front();
        q.pop();

        int d = dist[current_state];

        // Finde die kleinste Scheibe auf jedem Stab.
        // Nur die kleinste Scheibe eines Stabes darf bewegt werden.
        // Initialisierung mit 20 (groesser als jede moegliche Scheiben-ID).
        array<int, 4> tops = {20, 20, 20, 20};
        for (int i = 0; i < n; ++i) {
            int r = (current_state >> (2 * i)) & 3;
            if (tops[r] == 20) {
                tops[r] = i;
            }
        }

        // Versuche jede 'top' Scheibe von Stab 'from' auf Stab 'to' zu schieben.
        for (int from = 0; from < 4; ++from) {
            if (tops[from] == 20) continue; // Stab ist leer

            int disk = tops[from];

            for (int to = 0; to < 4; ++to) {
                if (from == to) continue;

                // Bewegung erlaubt, wenn Zielstab leer ist oder die 
                // oberste Scheibe dort groesser als die aktuelle Scheibe ist.
                if (tops[to] > disk) {
                    int next_state = current_state;
                    // Bits an Position 2*disk leeren.
                    next_state &= ~(3 << (2 * disk));
                    // Neuen Stab-Index 'to' an dieser Position setzen.
                    next_state |= (to << (2 * disk));

                    if (dist[next_state] == -1) {
                        // Da BFS den kuerzesten Weg garantiert, koennen wir 
                        // sofort abbrechen, wenn der Zielzustand erreicht wird.
                        if (next_state == target_state) return d + 1;
                        
                        dist[next_state] = d + 1;
                        q.push(next_state);
                    }
                }
            }
        }
    }

    return -1;
}

int main() {
    // Optimierung der Standard Ein- und Ausgabe
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int result = solve();
    if (result != -1) {
        cout << result << endl;
    }

    return 0;
}
