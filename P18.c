#include <stdio.h>

// Shift schedule for 16 rounds
int shiftSchedule[16] = {1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1};

// Left shift function for 28-bit halves
void leftShift(int *half, int shifts) {
    int temp[28];
    for (int i=0;i<28;i++) temp[i]=half[(i+shifts)%28];
    for (int i=0;i<28;i++) half[i]=temp[i];
}

// Generate subkeys: first 24 bits from C, second 24 bits from D
void generateSubkeys(int key[56], int subkeys[16][48]) {
    int C[28], D[28];
    // Split initial 56-bit key into halves
    for (int i=0;i<28;i++) C[i]=key[i];
    for (int i=0;i<28;i++) D[i]=key[i+28];

    for (int round=0; round<16; round++) {
        // Apply left shifts according to schedule
        leftShift(C, shiftSchedule[round]);
        leftShift(D, shiftSchedule[round]);

        // Construct subkey: 24 bits from C, 24 bits from D
        for (int i=0;i<24;i++) subkeys[round][i]=C[i];
        for (int i=0;i<24;i++) subkeys[round][i+24]=D[i];
    }
}

int main() {
    // Example 56-bit key (parity bits removed)
    int key[56];
    for (int i=0;i<56;i++) key[i]=i%2; // simple test pattern

    int subkeys[16][48];
    generateSubkeys(key, subkeys);

    // Print subkeys
    for (int r=0;r<16;r++) {
        printf("Round %2d subkey: ", r+1);
        for (int i=0;i<48;i++) printf("%d", subkeys[r][i]);
        printf("\n");
    }

    return 0;
}
