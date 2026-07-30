// Benchmark For Testing peformance of engine 
# include "mancala.h"
# include <stdio.h>

// Helper function to generate a string of legal moves
void GenerateExampleGameArray (int depth) { 

    struct GameState board = initBoard(); 

    int isP1Turn = 1; // Player 1 starts first

    printf("Array of Valid Moves: \n");

    printf("{ "); 
    while(!isGameOverFast(&board)) { 
        if(isP1Turn) { 
            int bestMove = getBestMove(&board, depth, true);
            printf("%d, ", bestMove);
            int ExtraMove = playMove(&board, bestMove, true);

            if (ExtraMove != 1) {
                isP1Turn = 0; // Switch to Player 2's turn
            }

        } else { 
            int bestMove = getBestMove(&board, depth, false);
            printf("%d, ", bestMove);
            int ExtraMove = playMove(&board, bestMove, false);
            if (ExtraMove != 1) {
                isP1Turn = 1; // Switch to Player 1's turn
            }
        }
        
    }
    printf("}\n");
}


int main() { 
    
    int BenchmarkGame1[] = { 2, 5, 11, 1, 5, 0, 10, 0, 8, 11, 10, 2, 9, 0, 10, 1, 11, 12, 1, 7, 0, 8, 1, 9 }; // Depth 10 generation
    int BenchmarkGame2[] = { 2, 4, 8, 7, 3, 8, 1, 11, 4, 3, 9, 1, 10, 4, 3, 2, 11, 12, 4, 7, 2, 10, 0, 8, 5, 12, 10, 4, 2, 11, 3, 1, 9, 0, 10, 4, 5, 9, 3 }; // Depth 11 generation
    int BenchmarkGame3[] = { 2, 4, 8, 11, 3, 10, 1, 9, 4, 2, 7, 0, 9, 3, 4, 5, 10, 0, 7, 3, 12, 3, 11, 2, 8, 1, 7, 2, 9, 0, 8, 1, 12, 11, 2, 12, }; // Depth 12 generation


    int depth = 10; // Set the desired depth for the minimax algorithm

    // Game 1 Benchmark
    printf("Benchmarking Game 1/3 \n");
    struct GameState board1 = initBoard();
    for (int i = 0; i < sizeof(BenchmarkGame1) / sizeof(BenchmarkGame1[0]); i++) {
        // Get best move to test peformance of minimax 
        getBestMove(&board1, depth, true);

        // Play next move in benchmark game to insure all benchmark games are the same length
        playMove(&board1, BenchmarkGame1[i], true);
    }

    // Game 2 Benchmark
    printf("Benchmarking Game 2/3 \n");
    struct GameState board2 = initBoard();
    for (int i = 0; i < sizeof(BenchmarkGame2) / sizeof(BenchmarkGame2[0]); i++) {
        getBestMove(&board2, depth, true);
        playMove(&board2, BenchmarkGame2[i], true);
    } 

    // Game 3 Benchmark
    printf("Benchmarking Game 3/3 \n");
    struct GameState board3 = initBoard();
    for (int i = 0; i < sizeof(BenchmarkGame3) / sizeof(BenchmarkGame3[0]); i++) {
        getBestMove(&board3, depth, true);
        playMove(&board3, BenchmarkGame3[i], true);
    }
    

}


// Depth 10 MiniMax + Alphabeta pruning: 13.68s 