#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cmath>
using namespace std;

#define WHITE 0
#define PEG 1
#define GOAL 2
#define NONE 3

#define MAX 222

int n, r, c, k;
pair<int, int> goal;
int board[MAX][MAX]; // Peg board
double sol[MAX][MAX]; // probability value for each cell
int visited[MAX][MAX]; // cell has been visited (for bfs)

// Create normal board with no missing pegs
void setup_board()
{
    for(int i = 0; i < r; i++){
        for(int j = 0; j < 2*c - 1; j++){
            if(i%2 == 0){
                if(j%2 == 0){
                    board[i][j] = PEG;
                } else {
                    board[i][j] = WHITE;
                } 
            } else {
                if((j == 0 || j == 2*c - 2)){
                    board[i][j] = NONE;
                } else if(j%2 == 0){
                    board[i][j] = WHITE; 
                } else {
                    board[i][j] = PEG;
                }
            }
        }
    }
}

// Test if it can go down from current state
bool goes_down(pair<int, int> st)
{
    return board[st.first + 1][st.second] == WHITE
        || board[st.first + 1][st.second] == GOAL;
}

// Test if it can go both sides from current state
bool goes_both_sides(pair<int, int> st)
{
    if(st.second - 1 < 0 || st.second + 1 == 2*c - 1
            || board[st.first + 1][st.second - 1] == NONE
            || board[st.first + 1][st.second + 1] == NONE
            || board[st.first + 1][st.second] == WHITE)
        return false;

    return true;
}

// Test if it can go right from current state
bool goes_right(pair<int, int> st)
{
    return board[st.first + 1][st.second + 1] == WHITE
        || board[st.first + 1][st.second + 1] == GOAL;
}

// Test if it can go left from current state
bool goes_left(pair<int, int> st)
{
    return board[st.first + 1][st.second - 1] == WHITE
        || board[st.first + 1][st.second - 1] == GOAL;
}


// Solves the board
pair<int, double> solve()
{
    int col_best = 0;
    double best = 0.0;
    pair<int, int> queue[MAX*MAX];

    // Try dropping the ball at every possible initial hole
    for(int i = 1; i < 2*c - 1; i += 2){
        int left = 0;
        int right = 0;
   
        // initialize
        for(int k = 0; k < MAX; k++) 
            for(int j = 0; j < MAX; j++){
                sol[k][j] = 0.0;
                visited[k][j] = 0;
            }
        
        // Where we are dropping the ball
        queue[right++] = make_pair(0, i);
        sol[0][i] = 1.0;

        // While queue not empty
        while(right != left){
            pair<int, int> st = queue[left++];

            if(st.first == r - 1){
                continue;
            }

            // Test to where we can go from current state
            if(goes_down(st)){
                sol[st.first + 1][st.second] += sol[st.first][st.second];
                if(!visited[st.first + 1][st.second]){
                    visited[st.first + 1][st.second] = 1;
                    queue[right++] = make_pair(st.first + 1, st.second);
                }
            } else if(goes_both_sides(st)){
                sol[st.first + 1][st.second - 1] += sol[st.first][st.second]/2.0;
                sol[st.first + 1][st.second + 1] += sol[st.first][st.second]/2.0;
                if(!visited[st.first + 1][st.second - 1]){
                    visited[st.first + 1][st.second - 1] = 1;
                    queue[right++] = make_pair(st.first + 1, st.second - 1);
                }
                if(!visited[st.first + 1][st.second + 1]){
                    visited[st.first + 1][st.second + 1] = 1;
                    queue[right++] = make_pair(st.first + 1, st.second + 1);
                }
            } else if(goes_left(st)){
                sol[st.first + 1][st.second - 1] += sol[st.first][st.second];
                if(!visited[st.first + 1][st.second - 1]){
                    visited[st.first + 1][st.second - 1] = 1;
                    queue[right++] = make_pair(st.first + 1, st.second - 1);
                }
            } else if(goes_right(st)){
                sol[st.first + 1][st.second + 1] += sol[st.first][st.second];
                if(!visited[st.first + 1][st.second + 1]){
                    visited[st.first + 1][st.second + 1] = 1;
                    queue[right++] = make_pair(st.first + 1, st.second + 1);
                }
            }
        }
        
        // Update best solution
        if(sol[goal.first][goal.second] > best){
            best = sol[goal.first][goal.second];
            col_best = (i - 1)/2;
        }
    }
    return make_pair(col_best, best);    
}


int main()
{
    scanf("%d", &n);

    for(int i = 0; i < n; i++){
        int m;
        scanf("%d %d %d %d", &r, &c, &k, &m);
        
        // create board
        setup_board();

        // set goal
        board[r - 1][2*k + 1] = GOAL;
        goal = make_pair(r - 1, 2*k + 1);

        // insert holes
        for(int j = 0; j < m; j++){
            int y, x;
            scanf("%d %d", &y, &x);
            board[y][2*x + (y%2 == 0 ? 0 : 1)] = WHITE; 
        }

        // Get solution
        pair<int, double> solution = solve();
        printf("%d %.6lf\n", solution.first, solution.second);
    }
    return 0;
}

