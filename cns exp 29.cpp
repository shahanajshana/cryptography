#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ROWS 5
#define COLS 5
#define LANE_SIZE 64
#define RATE_BITS 1024
#define CAPACITY_BITS 576

// Function to check if all capacity lanes have at least one nonzero bit
int all_nonzero(int state[ROWS][COLS]) {
    int start_lane = RATE_BITS / LANE_SIZE;
    for (int i = start_lane; i < ROWS * COLS; i++) {
        int row = i / COLS;
        int col = i % COLS;
        if (state[row][col] == 0) {
            return 0; // Found a zero lane
        }
    }
    return 1; // All capacity lanes have nonzero bits
}

// Function to simulate message absorption
void absorb_message(int state[ROWS][COLS], int message[ROWS][COLS]) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            state[i][j] ^= message[i][j];
        }
    }
}

int main() {
    int state[ROWS][COLS] = {0}; // Initialize all lanes to 0
    int message[ROWS][COLS];

    srand(time(NULL)); // Seed for random message generation

    // Generate the first message block (P0) with nonzero bits in all lanes
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            message[i][j] = rand() % (1ULL << LANE_SIZE); // Random 64-bit value
            if (message[i][j] == 0) {
                message[i][j] = 1; // Ensure at least one nonzero bit
            }
        }
    }

    int rounds = 0;
    printf("Starting SHA-3 simulation...\n");

    // Absorb message blocks until all capacity lanes are nonzero
    while (!all_nonzero(state)) {
        absorb_message(state, message);
        rounds++;

        // For demonstration, regenerate the message block for each round
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLS; j++) {
                message[i][j] = rand() % (1ULL << LANE_SIZE);
                if (message[i][j] == 0) {
                    message[i][j] = 1; // Ensure at least one nonzero bit
                }
            }
        }
    }

    printf("All capacity lanes have at least one nonzero bit after %d rounds.\n", rounds);

    return 0;
}


