#ifndef MANCALA_H
#define MANCALA_H

#include <stdbool.h>
#include <stdint.h>

#define MAX_SCORE 1000000
#define MIN_SCORE -1000000

// Define a struct to represent the board and the count of stones in each pit. 
struct GameState { 
    uint8_t pits[14];
}; 

// Function prototypes
struct GameState initBoard(void);
int printBoard(struct GameState board);
int playMove(struct GameState *board, uint8_t pitIndex, bool isPlayer1);
bool isGameOver(struct GameState *board);
bool isGameOverFast(struct GameState *board);
int evaluateBoard(struct GameState *board);
int minimax(struct GameState *board, int depth,int alpha, int beta, bool isMaximising);
int getBestMove(struct GameState *board, int depth, bool isMaximising);

#endif // MANCALA_H