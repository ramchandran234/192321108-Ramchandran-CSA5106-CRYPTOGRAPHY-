#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define SIZE 30

// Function to convert text to uppercase and replace 'J' with 'I'
void cleanText(char str[]) {
    int i, j = 0;
    for (i = 0; str[i] != '\0'; i++) {
        if (isalpha(str[i])) {
            char ch = toupper(str[i]);
            if (ch == 'J') ch = 'I';
            str[j++] = ch;
        }
    }
    str[j] = '\0';
}

// Function to generate the 5x5 key matrix
void generateKeyMatrix(char key[], char matrix[5][5]) {
    int map[26] = {0};
    map['J' - 'A'] = 1; // Treat J as I
    
    int r = 0, c = 0;
    
    // First, insert characters of the key
    for (int i = 0; key[i] != '\0'; i++) {
        int index = key[i] - 'A';
        if (!map[index]) {
            matrix[r][c] = key[i];
            map[index] = 1;
            c++;
            if (c == 5) { r++; c = 0; }
        }
    }
    
    // Fill the remaining matrix with remaining alphabet letters
    for (int i = 0; i < 26; i++) {
        if (!map[i]) {
            matrix[r][c] = (char)(i + 'A');
            map[i] = 1;
            c++;
            if (c == 5) { r++; c = 0; }
        }
    }
}

// Function to find the coordinates (row, col) of a character in the matrix
void findPosition(char matrix[5][5], char ch, int *row, int *col) {
    if (ch == 'J') ch = 'I';
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            if (matrix[i][j] == ch) {
                *row = i;
                *col = j;
                return;
            }
        }
    }
}

// Function to prepare the text into proper digraphs (pairs)
void preparePlainText(char str[], char prepared[]) {
    int i = 0, j = 0;
    while (str[i] != '\0') {
        prepared[j++] = str[i];
        // If two adjacent letters are identical, insert 'X' between them
        if (str[i+1] != '\0' && str[i] == str[i+1]) {
            prepared[j++] = 'X';
            i++;
        } else if (str[i+1] != '\0') {
            prepared[j++] = str[i+1];
            i += 2;
        } else {
            // If text length is odd, append 'X' at the end
            prepared[j++] = 'X';
            i++;
        }
    }
    prepared[j] = '\0';
}

// Function to encrypt pairs
void encryptPlayfair(char matrix[5][5], char text[]) {
    int r1, c1, r2, c2;
    for (int i = 0; text[i] != '\0'; i += 2) {
        findPosition(matrix, text[i], &r1, &c1);
        findPosition(matrix, text[i+1], &r2, &c2);

        // Rule 1: Same Row -> Shift columns right
        if (r1 == r2) {
            text[i] = matrix[r1][(c1 + 1) % 5];
            text[i+1] = matrix[r2][(c2 + 1) % 5];
        }
        // Rule 2: Same Column -> Shift rows down
        else if (c1 == c2) {
            text[i] = matrix[(r1 + 1) % 5][c1];
            text[i+1] = matrix[(r2 + 1) % 5][c2];
        }
        // Rule 3: Rectangle -> Swap columns
        else {
            text[i] = matrix[r1][c2];
            text[i+1] = matrix[r2][c1];
        }
    }
}

int main() {
    char key[SIZE], plaintext[SIZE * 2], preparedText[SIZE * 2];
    char matrix[5][5];

    printf("--- Playfair Cipher ---\n");
    printf("Enter the keyword: ");
    scanf("%s", key);
    
    printf("Enter the plaintext: ");
    // Clear buffer and read string with spaces
    getchar(); 
    fgets(plaintext, sizeof(plaintext), stdin);
    plaintext[strcspn(plaintext, "\n")] = '\0';

    // 1. Process inputs
    cleanText(key);
    cleanText(plaintext);
    
    // 2. Setup structural steps
    generateKeyMatrix(key, matrix);
    preparePlainText(plaintext, preparedText);

    printf("\nGenerated 5x5 Matrix:\n");
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            printf("%c ", matrix[i][j]);
        }
        printf("\n");
    }

    printf("\nPrepared Plaintext (Digraphs): %s\n", preparedText);

    // 3. Encrypt
    encryptPlayfair(matrix, preparedText);
    printf("Ciphertext: %s\n", preparedText);

    return 0;
}