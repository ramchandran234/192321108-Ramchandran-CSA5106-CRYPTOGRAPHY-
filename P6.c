#include <stdio.h>
#include <ctype.h>
#include <string.h>

// Function to find the modular multiplicative inverse
int modInverse(int a) {
    a = a % 26;
    for (int x = 1; x < 26; x++) {
        if ((a * x) % 26 == 1) return x;
    }
    return -1;
}

// Function to decrypt the ciphertext given keys a and b
void decryptAffine(char ciphertext[], int a, int b) {
    int a_inv = modInverse(a);
    if (a_inv == -1) {
        printf("Decryption impossible: 'a' has no modular inverse.\n");
        return;
    }

    printf("Decrypted Message: ");
    for (int i = 0; ciphertext[i] != '\0'; i++) {
        char ch = ciphertext[i];
        if (isupper(ch)) {
            int p = (a_inv * ((ch - 'A') - b + 26)) % 26;
            printf("%c", p + 'A');
        } else if (islower(ch)) {
            int p = (a_inv * ((ch - 'a') - b + 26)) % 26;
            printf("%c", p + 'a');
        } else {
            printf("%c", ch); // Keep spaces/punctuation
        }
    }
    printf("\n");
}

int main() {
    // Standard frequencies
    int p1 = 4;  // 'E'
    int p2 = 19; // 'T'
    
    // Ciphertext frequencies given in prompt
    int c1 = 1;  // 'B'
    int c2 = 20; // 'U'

    printf("--- Cracking Affine Cipher via Frequency Analysis ---\n");
    printf("Mapping: 'E' (4) -> 'B' (%d) and 'T' (19) -> 'U' (%d)\n\n", c1, c2);

    // 1. Solve: (p2 - p1) * a = (c2 - c1) mod 26
    int diff_p = (p2 - p1 + 26) % 26; // 19 - 4 = 15
    int diff_c = (c2 - c1 + 26) % 26; // 20 - 1 = 19

    int diff_p_inv = modInverse(diff_p); // Inverse of 15 mod 26 is 7
    
    if (diff_p_inv == -1) {
        printf("Error: Cannot determine unique keys mathematically.\n");
        return 1;
    }

    int a = (diff_c * diff_p_inv) % 26;
    
    // 2. Solve for b: b = (c1 - a * p1) mod 26
    int b = (c1 - (a * p1)) % 26;
    if (b < 0) b += 26; // Ensure positive remainder

    printf("Found Encryption Keys: a = %d, b = %d\n", a, b);
    printf("Found Decryption Keys: a^-1 = %d, b = %d\n\n", modInverse(a), b);

    // Input your ciphertext here to test decryption
    char ciphertext[500];
    printf("Enter the ciphertext to crack: ");
    fgets(ciphertext, sizeof(ciphertext), stdin);
    ciphertext[strcspn(ciphertext, "\n")] = '\0';

    decryptAffine(ciphertext, a, b);

    return 0;
}
