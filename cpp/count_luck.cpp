#include <iostream>
#include <vector>
#include <string>

using namespace std;

// Direction arrays for moving Up, Down, Left, Right
constexpr int dr[] = {-1, 1, 0, 0};
constexpr int dc[] = {0, 0, -1, 1};

/**
 * Depth First Search to find the Portkey and count the wand waves.
 * * @param r Current row
 * @param c Current column
 * @param pr Parent row (where we just came from)
 * @param pc Parent column
 * @param grid The forest matrix
 * @param visited 2D boolean array to keep track of visited cells
 * @return The number of wand waves required from this cell to the target, or -1 if dead end.
 */
int dfs(int r, int c, int pr, int pc, const vector<string>& grid, vector<vector<bool>>& visited) {
    // If we reached the Portkey, 0 wand waves are needed from this point onwards.
    if (grid[r][c] == '*') {
        return 0; 
    }

    visited[r][c] = true;
    int n = grid.size();
    int m = grid[0].size();
    int options = 0;

    // Count available valid paths (excluding the path we just came from)
    for (int i = 0; i < 4; ++i) {
        int nr = r + dr[i];
        int nc = c + dc[i];
        
        // Ensure the neighbor is within bounds and is not a tree ('X')
        if (nr >= 0 && nr < n && nc >= 0 && nc < m && grid[nr][nc] != 'X') {
            // Do not count the cell we arrived from as a new option
            if (nr != pr || nc != pc) {
                options++;
            }
        }
    }

    // Explore unvisited valid neighbors
    for (int i = 0; i < 4; ++i) {
        int nr = r + dr[i];
        int nc = c + dc[i];
        
        if (nr >= 0 && nr < n && nc >= 0 && nc < m && grid[nr][nc] != 'X' && !visited[nr][nc]) {
            int res = dfs(nr, nc, r, c, grid, visited);
            
            // If res is not -1, it means this branch successfully leads to the Portkey
            if (res != -1) {
                // Hermione waves her wand if there was more than 1 option at this current cell
                return res + (options > 1 ? 1 : 0);
            }
        }
    }

    // Returning -1 signifies this path was a dead end
    return -1; 
}

/**
 * Determines whether Ron's guess k is correct.
 * * @param matrix The grid map representing the forest.
 * @param k Ron's guess for the number of wand waves.
 * @return "Impressed" if the guess is correct, "Oops!" otherwise.
 */
string countLuck(vector<string> matrix, int k) {
    int n = matrix.size();
    int m = matrix[0].size();
    int startR = -1, startC = -1;

    // Find the starting coordinates of Hermione and Ron ('M')
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (matrix[i][j] == 'M') {
                startR = i;
                startC = j;
                break;
            }
        }
    }

    // Initialize visited array
    vector<vector<bool>> visited(n, vector<bool>(m, false));
    
    // Perform DFS starting from 'M'
    int totalWaves = dfs(startR, startC, -1, -1, matrix, visited);

    // Check if total calculated waves match Ron's guess
    return (totalWaves == k) ? "Impressed" : "Oops!";
}

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int t;
    if (!(cin >> t)) return 0;
    
    while (t--) {
        int n, m;
        cin >> n >> m;
        
        vector<string> matrix(n);
        for (int i = 0; i < n; ++i) {
            cin >> matrix[i];
        }
        
        int k;
        cin >> k;
        
        cout << countLuck(matrix, k) << "\n";
    }
    
    return 0;
}
