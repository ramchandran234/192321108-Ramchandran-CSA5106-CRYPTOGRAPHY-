#include <stdio.h>
#include <string.h>

#define MOD 26

// Function to compute modular inverse
int modInverse(int a) {
    a = a % MOD;
    for (int x = 1; x < MOD; x++) {
        if ((a * x) % MOD == 1) return x;
    }
    return -1; // no inverse
}

// Function to solve for key matrix given plaintext and ciphertext pairs
void recoverKey(int P[2][2], int C[2][2], int K[2][2]) {
    // Compute determinant of P
    int det = (P[0][0]*P[1][1] - P[0][1]*P[1][0]) % MOD;
    if (det < 0) det += MOD;
    int invDet = modInverse(det);

    // Inverse of P modulo 26
    int invP[2][2];
    invP[0][0] = ( P[1][1] * invDet) % MOD;
    invP[0][1] = (-P[0][1] * invDet) % MOD;
    invP[1][0] = (-P[1][0] * invDet) % MOD;
    invP[1][1] = ( P[0][0] * invDet) % MOD;
    for (int i=0;i<2;i++) for (int j=0;j<2;j++) if (invP[i][j]<0) invP[i][j]+=MOD;

    // K = C * invP
    for (int i=0;i<2;i++) {
        for (int j=0;j<2;j++) {
            K[i][j] = (C[i][0]*invP[0][j] + C[i][1]*invP[1][j]) % MOD;
        }
    }
}

int main() {
    // Example: Suppose we know plaintext "ME"->"UO" and "AT"->"XY"
    // Convert letters to numbers (A=0,...,Z=25)
    int P[2][2] = { {12,0}, {4,19} }; // columns are plaintext vectors
    int C[2][2] = { {20,23}, {14,24} }; // columns are ciphertext vectors

    int K[2][2];
    recoverKey(P,C,K);

    printf("Recovered Key Matrix:\n");
    for (int i=0;i<2;i++) {
        for (int j=0;j<2;j++) {
            printf("%d ", K[i][j]);
        }
        printf("\n");
    }

    return 0;
}
