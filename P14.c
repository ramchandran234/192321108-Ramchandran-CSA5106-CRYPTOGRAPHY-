#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MOD 26

// Encrypt function
void encrypt(const char *plaintext, int *key, char *ciphertext) {
    int len = strlen(plaintext);
    for (int i = 0; i < len; i++) {
        if (isalpha(plaintext[i])) {
            int p = toupper(plaintext[i]) - 'A';
            int c = (p + key[i]) % MOD;
            ciphertext[i] = c + 'A';
        } else {
            ciphertext[i] = plaintext[i]; // keep spaces/punctuation
        }
    }
    ciphertext[len] = '\0';
}

// Decrypt function
void decrypt(const char *ciphertext, int *key, char *plaintext) {
    int len = strlen(ciphertext);
    for (int i = 0; i < len; i++) {
        if (isalpha(ciphertext[i])) {
            int c = ciphertext[i] - 'A';
            int p = (c - key[i]) % MOD;
            if (p < 0) p += MOD;
            plaintext[i] = p + 'A';
        } else {
            plaintext[i] = ciphertext[i];
        }
    }
    plaintext[len] = '\0';
}

int main() {
    char message[] = "MEET ME AT THE USUAL PLACE AT TEN RATHER THAN EIGHT OCLOCK";
    int len = strlen(message);

    // Generate random key stream
    int key[len];
    srand(time(NULL));
    for (int i = 0; i < len; i++) {
        if (isalpha(message[i]))
            key[i] = rand() % MOD; // random shift 0–25
        else
            key[i] = 0; // no shift for spaces/punctuation
    }

    char ciphertext[200], decrypted[200];

    encrypt(message, key, ciphertext);
    decrypt(ciphertext, key, decrypted);

    printf("Plaintext:  %s\n", message);
    printf("Key stream: ");
    for (int i = 0; i < len; i++) {
        printf("%d ", key[i]);
    }
    printf("\nCiphertext: %s\n", ciphertext);
    printf("Decrypted:  %s\n", decrypted);

    return 0;
}
