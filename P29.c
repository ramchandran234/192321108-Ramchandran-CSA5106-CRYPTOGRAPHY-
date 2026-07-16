#include <stdio.h>
#include <stdint.h>

#define TOTAL_LANES 25
#define RATE_LANES 16      // 1024 bits / 64 bits per lane
#define CAPACITY_LANES 9   // 576 bits / 64 bits per lane

int main() {
    // SHA-3 State Matrix represented as 25 lanes of 64-bit integers
    uint64_t state[TOTAL_LANES] = {0}; // Initialized to all zeros

    // Plaintext block P0 (1024 bits -> 16 lanes)
    // Assume each lane has at least one non-zero bit (e.g., set to 0x1)
    uint64_t P0[RATE_LANES];
    for(int i = 0; i < RATE_LANES; i++) {
        P0[i] = 0x1ULL; 
    }

    printf("=== SHA-3 State Simulation (Ignoring Permutation) ===\n");
    printf("Total Lanes: %d | Rate Lanes: %d | Capacity Lanes: %d\n\n", 
            TOTAL_LANES, RATE_LANES, CAPACITY_LANES);

    // --- STEP 1: Absorb P0 into the Rate Lanes ---
    for(int i = 0; i < RATE_LANES; i++) {
        state[i] ^= P0[i];
    }
    printf("[Action] Absorbed first 1024-bit block (P0) into the rate portion.\n\n");

    // --- STEP 2: Evaluate Lanes ---
    int zero_capacity_lanes = 0;
    
    printf("--- State Analysis ---\n");
    for(int i = 0; i < TOTAL_LANES; i++) {
        if (i < RATE_LANES) {
            printf("Lane %02d (Rate)    : %s\n", i, 
                   state[i] != 0 ? "Has Non-Zero Bits" : "All Zeros");
        } else {
            printf("Lane %02d (Capacity): %s\n", i, 
                   state[i] != 0 ? "Has Non-Zero Bits" : "All Zeros");
            if(state[i] == 0) {
                zero_capacity_lanes++;
            }
        }
    }

    printf("\n=== Conclusion ===\n");
    if (zero_capacity_lanes > 0) {
        printf("Result: %d capacity lanes are still entirely composed of zeros.\n", zero_capacity_lanes);
        printf("Reason: Because the permutation function is ignored, data from the \n");
        printf("        rate lanes can never flow into or mix with the capacity lanes.\n");
        printf("Time Required: Infinite / Never.\n");
    } else {
        printf("All lanes have non-zero bits.\n");
    }

    return 0;
}