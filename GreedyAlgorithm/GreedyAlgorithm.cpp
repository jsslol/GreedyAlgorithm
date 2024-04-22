// Name: Jared Schneider
// Program Description: Implements a Greedy Algorithm to solve a modified version of the Policemen Catching Thieves Problem.
// Modification 1: A policeman can only catch a thief if both of them are in the same column. 
// Modification 2: A rookie can catch a thief in any row and column if a policeman can help catch them too in any row or column.
// Additional Modification 3: It can take 3 rookies to catch a thief in any row or column.
// Additional Modification 4: A policeman cannot catch a thief who is more than K units away in any column and row.  
#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <cmath>

using namespace std;

//CHANGE THESE TO TEST VALUES
const int MAX_N = 1000;
//Police+Thieve+Rookies must be equal to N*N
const int numPolice = 1;
const int numThieves = 10;
const int numRookies = 14;
const int N = 5; // Grid size
const int K = 3; // Maximum distance a policeman can catch a thief


// Function prototypes
void generateRandomMatrix(vector<vector<char>>& M, int N, int K,int numPolice, int numThieves, int numRookies);
void printMatrix(vector<vector<char>>& M, int N);
bool canCatchThief(int pRow, int pCol, int tRow, int tCol, int K);
void handlePolice(vector<vector<char>>& M, int N, int K, int i, int j, int& thievesCaught);
bool areThreeRookiesAdjacent(vector<vector<char>>& M, int N, int i, int j);
void handleRookie(vector<vector<char>>& M, int N, int K, int i, int j, int& thievesCaught);
void handleThief(vector<vector<char>>& M, int N, int i, int j, int& thievesCaught);
int solve_PCT(vector<vector<char>>& M, int N, int K);
bool checkBounds(int N, int K,int numPolice, int numThieves, int numRookies);

int main() {
    vector<vector<char>> M(MAX_N, vector<char>(MAX_N));

    // Solve the problem
    int T = solve_PCT(M, N, K);

    // Print the result
    cout << endl << T << " thieves were caught." << endl;

    return 0;
}

// Function to solve the Policemen Catching Thieves problem
int solve_PCT(vector<vector<char>>& M, int N, int K) {
    srand(time(0)); // Seed the random number generator with current time
    int thievesCaught = 0;

    // Generate a random input grid matrix
    generateRandomMatrix(M, N,K,numPolice,numThieves,numRookies);

    // Print the random grid matrix
    cout << "Random Input Grid:" << endl;
    printMatrix(M, N);

    // Loop through each cell in the grid
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (M[i][j] == 'P') {
                // Current cell contains a policeman
                handlePolice(M, N, K, i, j, thievesCaught);
            } else if (M[i][j] == 'R') {
                // Current cell contains a rookie
                handleRookie(M, N, K, i, j, thievesCaught);
            } else if (M[i][j] == 'T') {
                // Current cell contains a thief
                handleThief(M, N, i, j, thievesCaught);
            }
        }
    }

    return thievesCaught;
}

// Function to generate a random grid matrix with specified numbers of Police, Thieves, and Rookies
void generateRandomMatrix(vector<vector<char>>& M, int N, int K,int numPolice, int numThieves, int numRookies) {
    bool check = checkBounds(N,K,numPolice,numThieves,numRookies);
    if(check == false){
        exit(0);
    }

    // Initialize the grid with empty cells
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            M[i][j] = '.';
        }
    }

    // Place Police
    while (numPolice > 0) {
        int i = rand() % N;
        int j = rand() % N;
        if (M[i][j] == '.') {
            M[i][j] = 'P';
            numPolice--;
        }
    }

    // Place Thieves
    while (numThieves > 0) {
        int i = rand() % N;
        int j = rand() % N;
        if (M[i][j] == '.') {
            M[i][j] = 'T';
            numThieves--;
        }
    }

    // Place Rookies
    while (numRookies > 0) {
        int i = rand() % N;
        int j = rand() % N;
        if (M[i][j] == '.') {
            M[i][j] = 'R';
            numRookies--;
        }
    }
}

//Handles Edge Cases
bool checkBounds(int N, int K,int numPolice, int numThieves, int numRookies) {
    // Ensure that the total number of agents does not exceed the grid size
    int total = numPolice + numThieves + numRookies;

    if(N > MAX_N || N < 1){
        cerr << "Error: N must be between 1-"<<MAX_N<<"." << endl << "Current N: " << N << endl;
        return false;        
    }
    if (K > N*N || K < 1) {
        cerr << "Error: Total number of agents exceeds grid size." << endl;
        return false;
    }
    if (numThieves < 1 || numThieves >10 ) {
        cerr << "Error: There must be 1-10 thieves." << endl << "Current Thieves: " << numThieves << endl;
        return false;
    }
    if (total != N*N) {
        cerr << "Error: Police(" << numPolice << ") + Thieves(" << numThieves << ") + Rookies(" << numRookies <<") must equal N*N("<<(N*N)<<")." << endl << 
            "Current Total: " << total << endl;
        return false;
    }

    return true;
}


// Function to check if a policeman can catch a thief in the same column and within K units
bool canCatchThief(int pRow, int pCol, int tRow, int tCol, int K) {
    return (pCol == tCol && abs(pRow - tRow) <= K);
}

// Function to handle police actions
void handlePolice(vector<vector<char>>& M, int N, int K, int i, int j, int& thievesCaught) {
    for (int x = 0; x < N; ++x) {
        for (int y = 0; y < N; ++y) {
            if (M[x][y] == 'T' && canCatchThief(i, j, x, y, K)) {
                // Policeman can catch the thief
                M[x][y] = 'C'; // Mark the thief as caught
                cout << "Thief at (" << x + 1 << "," << y + 1 << ") was caught by Police at (" << i + 1 << "," << j + 1 << ")" << endl;
                thievesCaught++;
                return; // Each policeman catches only one thief
            }
        }
    }
}

// Function to check if there are three rookies directly adjacent (up, down, left, or right) to a cell
bool areThreeRookiesAdjacent(vector<vector<char>>& M, int N, int i, int j) {
    int recruitsAdjacent = 0;

    // Check if there are three rookies adjacent (up, down, left, or right) to the cell
    if (i > 0 && M[i - 1][j] == 'R') {
        recruitsAdjacent++;
    }
    if (i < N - 1 && M[i + 1][j] == 'R') {
        recruitsAdjacent++;
    }
    if (j > 0 && M[i][j - 1] == 'R') {
        recruitsAdjacent++;
    }
    if (j < N - 1 && M[i][j + 1] == 'R') {
        recruitsAdjacent++;
    }

    return (recruitsAdjacent >= 3);
}


// Function to handle rookie actions
void handleRookie(vector<vector<char>>& M, int N, int K, int i, int j, int& thievesCaught) {
    int thiefX = -1;
    int thiefY = -1;

    // Check if there is a thief directly adjacent (up, down, left, or right) to the rookie
    if (i > 0 && M[i - 1][j] == 'T') {
        thiefX = i - 1;
        thiefY = j;
    } else if (i < N - 1 && M[i + 1][j] == 'T') {
        thiefX = i + 1;
        thiefY = j;
    } else if (j > 0 && M[i][j - 1] == 'T') {
        thiefX = i;
        thiefY = j - 1;
    } else if (j < N - 1 && M[i][j + 1] == 'T') {
        thiefX = i;
        thiefY = j + 1;
    }

    if (thiefX != -1 && thiefY != -1) {
        for (int x = 0; x < N; ++x) {
            for (int y = 0; y < N; ++y) {
                if (M[x][y] == 'P' && canCatchThief(x, y, thiefX, thiefY, 1)) {
                    // Rookie can catch the thief with a policeman directly adjacent (up, down, left, or right) to the thief
                    M[thiefX][thiefY] = 'C'; // Mark the thief as caught
                    cout << "Thief at (" << thiefX + 1 << "," << thiefY + 1 << ") was caught by Rookie at (" << i + 1 << "," << j + 1 
                            << ") with Police at (" << x + 1 << "," << y + 1 << ")" << endl;
                    thievesCaught++;
                    return; // Each rookie catches only one thief
                }
            }
        }
    }
}

// Function to handle thief actions
void handleThief(vector<vector<char>>& M, int N, int i, int j, int& thievesCaught) {
    if (areThreeRookiesAdjacent(M, N, i, j)) {
        // Three rookies adjacent to the thief can catch it without help from a policeman
        M[i][j] = 'C'; // Mark the thief as caught
        cout << "Thief at (" << i + 1 << "," << j + 1 << ") was caught by 3 Recruits" << endl;
        thievesCaught++;
    }
}

// Function to print the grid matrix
void printMatrix(vector<vector<char>>& M, int N) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            cout << M[i][j] << " ";
        }
        cout << endl;
    }
}