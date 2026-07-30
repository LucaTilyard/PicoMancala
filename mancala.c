#include <stdio.h>
#include "mancala.h"



// Initalises the board with 4 stones in each pit and 0 in the stores.
struct GameState initBoard() { 

    struct GameState board = 
    { 4, 4, 4, 4, 4, 4, 0,
      4, 4, 4, 4, 4, 4, 0 };

    return board;
}

// prints the current state of the board. 
int printBoard(struct GameState board) {
    printf("    %d  %d  %d  %d  %d  %d\n", board.pits[12], board.pits[11], board.pits[10], board.pits[9], board.pits[8], board.pits[7]);
    printf("%d                          %d\n", board.pits[13], board.pits[6]);
    printf("    %d  %d  %d  %d  %d  %d\n", board.pits[0], board.pits[1], board.pits[2], board.pits[3], board.pits[4], board.pits[5]);
    return 0;
}

// Peforms a single turn of the game. 
int playMove(struct GameState *board, uint8_t pitIndex, bool isPlayer1) { 

    // Pick up stones from the selected pit
    uint8_t stones_in_hand = board -> pits[pitIndex];

    // Empty pit stones have been picked up from
    board -> pits[pitIndex] = 0;

    // Distribute stones to subsequent pits
    uint8_t currentIndex = pitIndex;
    while(stones_in_hand > 0) { 
        currentIndex = (currentIndex + 1) % 14;
        
        if (isPlayer1 && currentIndex == 13) {
            continue; // Skip opponent's store
        } 
        
        if (!isPlayer1 && currentIndex == 6) {
            continue; // Skip opponent's store
        }

        board->pits[currentIndex]++;
        stones_in_hand--;
    }

    // Check for captures
    uint8_t oppositeIndex = 12 - currentIndex;
    if (isPlayer1 && currentIndex >= 0 && currentIndex <= 5 && board->pits[currentIndex] == 1 && board->pits[oppositeIndex] > 0) {
        board->pits[6] += board->pits[oppositeIndex] + 1; // Capture stones
        board->pits[oppositeIndex] = 0;
        board->pits[currentIndex] = 0;
    } else if (!isPlayer1 && currentIndex >= 7 && currentIndex <= 12 && board->pits[currentIndex] == 1 && board->pits[oppositeIndex] > 0) {
        board->pits[13] += board->pits[oppositeIndex] + 1; // Capture stones
        board->pits[oppositeIndex] = 0;
        board->pits[currentIndex] = 0;
    }


    // Check for extra turns, returns 1 for extra turn, 0 for normal turn end

    if (isPlayer1 && currentIndex == 6) return 1; // Player 1 gets another turn
    if (!isPlayer1 && currentIndex == 13) return 1; // Player 2 gets another turn
    return 0; // Normal turn end, no extra turns. 
    
}

// Checks board state to see if the game is over. If game is over peforms "sweep" to calculate final scores. Returns true if game is over, else false
bool isGameOver(struct GameState *board){ 

    // Check if either player has 0 stones in their pits
    int p1_stones = 0;
    int p2_stones = 0;

    for  (int i = 0; i <= 5; i++){ 
        p1_stones += board ->pits[i];
    }

    for (int i = 7; i <= 12; i++){ 
        p2_stones += board ->pits[i]; 
    }

    // Perform sweep and return true if game is over
    if (p1_stones == 0 || p2_stones == 0) { 
        board -> pits[6] += p1_stones; 
        board -> pits[13] += p2_stones; 
        return true; 
    }

    // else game continues
    return false; 
}

// Checks board state to see if game is over but does not peform the "sweep".
bool isGameOverFast(struct GameState *board){ 

    // Check if either player has 0 stones in their pits
    int p1_stones = 0;
    int p2_stones = 0;

    for  (int i = 0; i <= 5; i++){ 
        p1_stones += board ->pits[i];
    }

    for (int i = 7; i <= 12; i++){ 
        p2_stones += board ->pits[i]; 
    }

    if (p1_stones == 0 || p2_stones == 0) { 
        return true; 
    }

    return false;
}

// Evaluates the board state and returns a score, specify the size of this int ?? 
int evaluateBoard(struct GameState *board) { 

    // Positive is good for player 1, negative is good for player 2
    int p1_score = board -> pits[6];
    int p2_score = board -> pits[13];

    // THE SWEEP:
    // sweep is counted to improve heuristic evaluation of board. 
    // if this is removed care must be taken to switch to standard isGameOver() in the minimax function.
    // Both should be compared to see if the sweep improves the heuristic evaluation of the board. and at what cost to performance.
    for (int i = 0; i <= 5; i++) p1_score += board->pits[i];
    for (int i = 7; i <= 12; i++) p2_score += board->pits[i];

    return p1_score - p2_score;
}

// implement vanilla minimax algorithm to find best move for current player. Returns the best score for the current player.
int minimax(struct GameState *board, int depth, int alpha, int beta, bool isMaximising){ 

    // End of tree: if game is over or depth is 0, evaluate the board position
    if (depth == 0 || isGameOver(board)) { 
        return evaluateBoard(board); 
    }

    // Maximising player (Player 1)
    if (isMaximising) {
        int bestScore = MIN_SCORE;
        for( int i = 0; i <= 5; i++) { 
            if (board -> pits[i] == 0) continue; // Skip empty pits

            // Make temporeary copy of the board
            struct GameState newBoard = *board; 

            // Play the move and check for extra turn
            int extraTurn = playMove(&newBoard, i, true); 

            int currentScore;
            if (extraTurn == 1) { 
                currentScore = minimax(&newBoard, depth, alpha, beta, isMaximising); 
            } else { 
                currentScore = minimax(&newBoard, depth - 1, alpha, beta, !isMaximising); 
            } 

            // Keep the highest score
            if (currentScore > bestScore) {
                bestScore = currentScore;
            }

            // Alpha-beta pruning 
            if (bestScore > alpha){ 
                alpha = bestScore;
            }

            if (beta <= alpha) { 
                break; // Prune the remaing branches
            }

        }
        return bestScore;
    } else { // Minimising player (Player 2)
        int bestScore = MAX_SCORE;
        for( int i = 7; i <= 12; i++) { 
            if (board -> pits[i] == 0) continue; // Skip empty pits

            // Make temporeary copy of the board
            struct GameState newBoard = *board; 

            // Play the move and check for extra turn
            int extraTurn = playMove(&newBoard, i, false); 

            int currentScore;
            if (extraTurn == 1) { 
                currentScore = minimax(&newBoard, depth, alpha, beta, isMaximising); 
            } else { 
                currentScore = minimax(&newBoard, depth - 1, alpha, beta, !isMaximising); 
            } 

            // Keep the lowest score
            if (currentScore < bestScore) {
                bestScore = currentScore;
            }

            // Alpha-beta pruning
            if (bestScore < beta){ 
                beta = bestScore;
            }

            if (beta <= alpha) { 
                break; // Prune the remaing branches
            }
        }
        return bestScore;
    }
}

// Minimax wrapper that finds the best move for the currant player. Returns the index of the best pit to play.
int getBestMove(struct GameState *board, int depth, bool isMaximising) { 
    int bestMove = -1;
    int bestScore = isMaximising ? MIN_SCORE : MAX_SCORE;

    if (isMaximising) {
        for (int i = 0; i <= 5; i++) {
            if (board->pits[i] == 0) continue; // Skip empty pits

            struct GameState newBoard = *board;
            int extraTurn = playMove(&newBoard, i, true);
            int currentScore;

            if (extraTurn == 1) {
                currentScore = minimax(&newBoard, depth, MIN_SCORE, MAX_SCORE, isMaximising);
            } else {
                currentScore = minimax(&newBoard, depth - 1, MIN_SCORE, MAX_SCORE, !isMaximising);
            }

            if (currentScore > bestScore) {
                bestScore = currentScore;
                bestMove = i;
            }
        }
    } else {
        for (int i = 7; i <= 12; i++) {
            if (board->pits[i] == 0) continue; // Skip empty pits

            struct GameState newBoard = *board;
            int extraTurn = playMove(&newBoard, i, false);
            int currentScore;

            if (extraTurn == 1) {
                currentScore = minimax(&newBoard, depth, MIN_SCORE, MAX_SCORE, isMaximising);
            } else {
                currentScore = minimax(&newBoard, depth - 1, MIN_SCORE, MAX_SCORE, !isMaximising);
            }

            if (currentScore < bestScore) {
                bestScore = currentScore;
                bestMove = i;
            }
        }
    }

    return bestMove;
}