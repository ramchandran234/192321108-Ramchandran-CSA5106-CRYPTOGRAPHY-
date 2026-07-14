#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MOD 26

// Key matrix
int key[2][2] = { {9,4}, {5,7} };

// Function to encrypt a pair
void encryptPair(int a, int b, int *c1, int *c2) {
    *c1 = (key[0][0]*a + key[0][1]*b) % MOD;
    *c2 = (key[1][0]*a + key[1][1]*b) % MOD;
}

// Function to decrypt a pair
void decryptPair(int a, int b, int *p1, int *p2) {
    // Compute inverse of key matrix modulo 26
    int det = (key[0][0]*key[1][1] - key[0][1]*key[1][0]) % MOD;
    if (det < 0) det += MOD;

    // Find multiplicative inverse of det mod 26
    int invDet = -1;
    for (int i = 1; i < MOD; i++) {
        if ((det * i) % MOD == 1) {
            invDet = i;
            break;
        }
    }

    int invKey[2][2];
    invKey[0][0] = ( key[1][1] * invDet) % MOD;
    invKey[0][1] = (-key[0][1] * invDet) % MOD;
    invKey[1][0] = (-key[1][0] * invDet) % MOD;
    invKey[1][1] = ( key[0][0] * invDet) % MOD;

    // Fix negatives
    for (int i=0;i<2;i++)
        for (int j=0;j<2;j++)
            if (invKey[i][j] < 0) invKey[i][j] += MOD;

    *p1 = (invKey[0][0]*a + invKey[0][1]*b) % MOD;
    *p2 = (invKey[1][0]*a + invKey[1][1]*b) % MOD;
}

int main() {
    char msg[] = "meet me at the usual place at ten rather than eight oclock";
    char prepared[200], encrypted[200], decrypted[200];
    int len=0;

    // Prepare message: remove spaces, lowercase to uppercase
    for (int i=0; msg[i]; i++) {
        if (isalpha(msg[i])) {
            prepared[len++] = toupper(msg[i]);
        }
    }
    if (len % 2 != 0) prepared[len++] = 'X'; // pad if odd
    prepared[len] = '\0';

    // Encrypt
    int epos=0;
    for (int i=0; i<len; i+=2) {
        int a = prepared[i]-'A';
        int b = prepared[i+1]-'A';
        int c1, c2;
        encryptPair(a,b,&c1,&c2);
        encrypted[epos++] = c1+'A';
        encrypted[epos++] = c2+'A';
    }
    encrypted[epos] = '\0';

    // Decrypt
    int dpos=0;
    for (int i=0; i<epos; i+=2) {
        int a = encrypted[i]-'A';
        int b = encrypted[i+1]-'A';
        int p1, p2;
        decryptPair(a,b,&p1,&p2);
        decrypted[dpos++] = p1+'A';
        decrypted[dpos++] = p2+'A';
    }
    decrypted[dpos] = '\0';

    printf("Plaintext: %s\n", prepared);
    printf("Ciphertext: %s\n", encrypted);
    printf("Decrypted: %s\n", decrypted);

    return 0;
}
