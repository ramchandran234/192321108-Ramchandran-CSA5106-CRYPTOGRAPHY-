#include <stdio.h>
#include <ctype.h>
#include <string.h>

// Function to find the Greatest Common Divisor (GCD)
int gcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// Function to find the Modular Multiplicative Inverse of 'a' modulo 26
// That is, find 'x' such that (a * x) % 26 == 1
int modInverse(int a) {
    a = a % 26;
    for (int x = 1; x < 26; x++) {
        if ((a * x) % 26 == 1) {
            return x;
        }
    }
    return -1; // Should not be reached if 'a' is validated as coprime
}

// Encryption function: C = (a * p + b) mod 26
void encryptAffine(char text[], int a, int b) {
    for (int i = 0; text[i] != '\0'; i++) {
        char ch = text[i];
        if (isupper(ch)) {
            text[i] = ((a * (ch - 'A') + b) % 26 + 26) % 26 + 'A';
        } else if (islower(ch)) {
            text[i] = ((a * (ch - 'a') + b) % 26 + 26) % 26 + 'a';
        }
    }
}

// Decryption function: p = a^-1 * (C - b) mod 26
void decryptAffine(char text[], int a, int b) {
    int a_inv = modInverse(a);
    if (a_inv == -1) return;

    for (int i = 0; text[i] != '\0'; i++) {
        char ch = text[i];
        if (isupper(ch)) {
            text[i] = ((a_inv * ((ch - 'A') - b)) % 26 + 26) % 26 + 'A';
        } else if (islower(ch)) {
            text[i] = ((a_inv * ((ch - 'a') - b)) % 26 + 26) % 26 + 'a';
        }
    }
}

int main() {
    char text[500];
    int a, b;

    printf("--- Affine Caesar Cipher ---\n");
    printf("Enter plaintext: ");
    fgets(text, sizeof(text), stdin);
    text[strcspn(text, "\n")] = '\0';

    printf("Enter key 'a': ");
    scanf("%d", &a);
    printf("Enter key 'b': ");
    scanf("%d", &b);

    // Validate if 'a' is coprime with 26
    if (gcd(a, 26) != 1) {
        printf("\nError: 'a' must be coprime to 26 (i.e., GCD(a, 26) must be 1).\n");
        printf("Values of 'a' allowed: 1, 3, 5, 7, 9, 11, 15, 17, 19, 21, 23, 25.\n");
        return 1;
    }

    encryptAffine(text, a, b);
    printf("\nCiphertext: %s\n", text);

    decryptAffine(text, a, b);
    printf("Decrypted Plaintext: %s\n", text);

    return 0;
}