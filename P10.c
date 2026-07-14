#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define SIZE 5

// Playfair matrix provided
char matrix[SIZE][SIZE] = {
    {'M','F','H','I','K'},
    {'U','N','O','P','Q'},
    {'Z','V','W','X','Y'},
    {'E','L','A','R','G'},
    {'D','S','T','B','C'}
};

// Function to find position of a character in matrix
void findPosition(char ch, int *row, int *col) {
    if (ch == 'J') ch = 'I'; // I/J treated same
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (matrix[i][j] == ch) {
                *row = i;
                *col = j;
                return;
            }
        }
    }
}

// Encryption of digraph
void encryptPair(char a, char b, char *c1, char *c2) {
    int r1, c1pos, r2, c2pos;
    findPosition(a, &r1, &c1pos);
    findPosition(b, &r2, &c2pos);

    if (r1 == r2) { // Same row
        *c1 = matrix[r1][(c1pos + 1) % SIZE];
        *c2 = matrix[r2][(c2pos + 1) % SIZE];
    } else if (c1pos == c2pos) { // Same column
        *c1 = matrix[(r1 + 1) % SIZE][c1pos];
        *c2 = matrix[(r2 + 1) % SIZE][c2pos];
    } else { // Rectangle
        *c1 = matrix[r1][c2pos];
        *c2 = matrix[r2][c1pos];
    }
}

int main() {
    char message[] = "Must see you over Cadogan West Coming at once";
    char prepared[200], encrypted[200];
    int len = 0;

    // Prepare message: remove spaces, punctuation, uppercase
    for (int i = 0; message[i]; i++) {
        if (isalpha(message[i])) {
            prepared[len++] = toupper(message[i]);
        }
    }
    prepared[len] = '\0';

    // Insert X between duplicate letters in digraphs
    char digraphs[200];
    int dlen = 0;
    for (int i = 0; i < len; i++) {
        digraphs[dlen++] = prepared[i];
        if (i < len - 1 && prepared[i] == prepared[i+1]) {
            digraphs[dlen++] = 'X';
        }
    }
    if (dlen % 2 != 0) digraphs[dlen++] = 'X'; // pad if odd length
    digraphs[dlen] = '\0';

    // Encrypt digraphs
    int epos = 0;
    for (int i = 0; i < dlen; i += 2) {
        char c1, c2;
        encryptPair(digraphs[i], digraphs[i+1], &c1, &c2);
        encrypted[epos++] = c1;
        encrypted[epos++] = c2;
    }
    encrypted[epos] = '\0';

    printf("Original Message: %s\n", message);
    printf("Prepared Message: %s\n", digraphs);
    printf("Encrypted Message: %s\n", encrypted);

    return 0;
}
