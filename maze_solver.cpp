#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <unordered_map>
#include <algorithm>
using namespace std;
//hello
#define N 6  // Size of the maze (can be adjusted)

// Global maze variable
int maze[N][N] = { { 1, 0, 0, 1, 0, 0 },
                   { 1, 1, 1, 1, 1, 0 },
                   { 0, 1, 0, 1, 0, 0 },
                   { 1, 1, 0, 1, 1, 0 },
                   { 1, 0, 0, 0, 1, 0 },
                   { 1, 1, 1, 1, 1, 1 } };

void printMaze0() {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cout << maze[i][j] << " ";
        }
        cout << endl;
    }
}

//backtracking
// A utility function to check if x, y is a valid index for N*N maze
bool isSafe1(int x, int y) {
    // Check if x, y are within bounds and the cell is not blocked
    return (x >= 0 && x < N && y >= 0 && y < N && maze[x][y] == 1);
}

// Recursive utility function to solve the maze using backtracking
bool solveMazeUtil1(int x, int y) {
    // Base case: if we've reached the bottom-right corner
    if (x == N - 1 && y == N - 1 && maze[x][y] == 1) {
        maze[x][y] = 3;  // Mark the destination as part of the path
        return true;
    }

    // Check if maze[x][y] is a valid move
    if (isSafe1(x, y)) {
        // Mark the current cell as part of the solution path
        maze[x][y] = 3;

        // Move down in the y direction
        if (solveMazeUtil1(x + 1, y))
            return true;

        // Move forward in the x direction
        if (solveMazeUtil1(x, y + 1))
            return true;

        // If neither forward nor down works, mark it as a dead-end and backtrack
        maze[x][y] = 2;
        return false;
    }

    return false;
}

// Solves the maze problem using backtracking
bool solveMaze1() {
    if (!solveMazeUtil1(0, 0)) {
        cout << "No solution found." << endl;
        return false;
    }

    // Print the maze with the final path markings
    cout << "Backtracking:" << endl;
    printMaze0();
    return true;
}
//bactracking ends

//deadend filling
// Function to check if the cell is valid
bool isSafe3(int x, int y) {
    return (x >= 0 && x < N && y >= 0 && y < N && maze[x][y] == 1);
}

// Recursive utility function to solve the maze using Dead-End Filling
bool solveMazeUtil3(int x, int y) {
    // If (x, y) is the bottom-right corner, return true (exit found)
    if (x == N - 1 && y == N - 1) {
        maze[x][y] = 3;  // Mark the end point as part of the final path
        return true;
    }

    // Check if maze[x][y] is a valid move
    if (isSafe3(x, y)) {
        // Mark this cell as part of the solution path
        maze[x][y] = 3; // Mark as part of the final path

        // Move in all four directions: right, down, left, up
        if (solveMazeUtil3(x + 1, y)) return true; // Move down
        if (solveMazeUtil3(x, y + 1)) return true; // Move right
        if (solveMazeUtil3(x - 1, y)) return true; // Move up
        if (solveMazeUtil3(x, y - 1)) return true; // Move left

        // If none of the above moves work out, unmark this cell (dead end)
        maze[x][y] = 2; // Mark as visited but not part of the solution
        return false;
    }

    return false;
}



// Function to print the maze
void printMaze3() {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++)
            cout << maze[i][j] << " ";
        cout << endl;
    }
}

// Function to check if any solution path exists
bool hasSolutionPath() {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (maze[i][j] == 3) {  // If any cell is part of the solution path
                return true;
            }
        }
    }
    return false;
}

// Function to solve the maze
void solveMaze3() {
    solveMazeUtil3(0, 0);  // Attempt to solve the maze

    // Check if any path was found
    if (!hasSolutionPath()) {
        cout << "No solution found." << endl;
    }
}

//deadend filling ends

//A*
struct Cell {
    int row, col;
    float f, g, h;
    Cell* parent;
    
    Cell(int r, int c, float g_score, float h_score, Cell* p = nullptr)
        : row(r), col(c), g(g_score), h(h_score), parent(p) {
        f = g + h;
    }
};

struct Compare {
    bool operator()(Cell* a, Cell* b) {
        return a->f > b->f;  // Compare by f-score
    }
};

int ROWS = 6;
int COLS = 6;

vector<pair<int, int>> directions = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} };

// Heuristic function: Manhattan distance
float heuristic(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}

bool isValid4(int row, int col) {
    return row >= 0 && row < ROWS && col >= 0 && col < COLS && maze[row][col] == 1;
}

void markWrongPath() {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (maze[i][j] == 1) maze[i][j] = 2;  // Mark visited cells with 2 (not in the solution)
        }
    }
}

void markSolutionPath(Cell* goal) {
    while (goal) {
        maze[goal->row][goal->col] = 3;  // Mark the correct path with 3
        goal = goal->parent;
    }
}

vector<pair<int, int>> a_star(int startRow, int startCol, int goalRow, int goalCol) {
    priority_queue<Cell*, vector<Cell*>, Compare> openSet;
    unordered_map<int, Cell*> allCells;

    // Initialize the start cell
    Cell* start = new Cell(startRow, startCol, 0, heuristic(startRow, startCol, goalRow, goalCol));
    openSet.push(start);
    allCells[startRow * COLS + startCol] = start;

    while (!openSet.empty()) {
        Cell* current = openSet.top();
        openSet.pop();

        // If goal is reached, reconstruct and return the path
        if (current->row == goalRow && current->col == goalCol) {
            markWrongPath();      // Mark all visited cells
            markSolutionPath(current);  // Mark the correct path
            return {};  // Path found
        }

        for (auto& dir : directions) {
            int newRow = current->row + dir.first;
            int newCol = current->col + dir.second;

            if (isValid4(newRow, newCol)) {
                float new_g = current->g + 1;
                float new_h = heuristic(newRow, newCol, goalRow, goalCol);
                int key = newRow * COLS + newCol;

                if (allCells.find(key) == allCells.end() || new_g < allCells[key]->g) {
                    Cell* neighbor = new Cell(newRow, newCol, new_g, new_h, current);
                    openSet.push(neighbor);
                    allCells[key] = neighbor;

                    maze[newRow][newCol] = 1;  // Mark cell as visited but not yet confirmed as a solution
                }
            }
        }
    }

    return {};  // No path found
}

void printMaze4() {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            cout << maze[i][j] << " ";
        }
        cout << endl;
    }
}
//A* ends

int main()
{
    cout << "Initial Maze:" << endl;
    printMaze0();

    //backtracking
    solveMaze1();

    //deadend filling
    solveMaze3();
    
    cout << "Deadend filling:" << endl;
    printMaze3();

    //A*
    int startRow = 0, startCol = 0;
    int goalRow = 5, goalCol = 5;

    a_star(startRow, startCol, goalRow, goalCol);

    cout << "A*:" << endl;
    printMaze4();

    return 0;
}
