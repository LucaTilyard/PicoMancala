#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include "mancala.h"
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

#define MAX_SCORE 1000000
#define MIN_SCORE -1000000


int main()
{
    stdio_init_all();

    // Initialise the Wi-Fi chip
    if (cyw43_arch_init()) {
        printf("Wi-Fi init failed\n");
        return -1;
    }
    struct GameState board = initBoard();

    while (true) {
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
        int bestMove = getBestMove(&board, 10, true);
        printf("Best move for Player 1: %d\n", bestMove);
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);

    }
}

// Refactor 
// implement iterative deepening, use this to improve move order. 
// Threading: Use threads to run minimax in parallel

