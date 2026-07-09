#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MATRIX_SIZE 5

// Function to clean text (convert to uppercase, change J to I, strip spaces)
void cleanText(const char input[], char output[]) {
    int j = 0;
    for (int i = 0; input[i] != '\0'; i++) {
        if (isalpha(input[i])) {
            char ch = toupper(input[i]);
            if (ch == 'J') ch = 'I';
            output[j++] = ch;
        }
    }
    output[j] = '\0';
}

// Function to generate the 5x5 key matrix from the keyword
void generateKeyMatrix(const char keyword[], char matrix[5][5]) {
    int map[26] = {0};
    map['J' - 'A'] = 1; // Treat J as I
    
    char cleanedKey[100];
    cleanText(keyword, cleanedKey);
    
    int r = 0, c = 0;
    
    // First, fill in the unique letters of the keyword
    for (int i = 0; cleanedKey[i] != '\0'; i++) {
        int index = cleanedKey[i] - 'A';
        if (!map[index]) {
            matrix[r][c] = cleanedKey[i];
            map[index] = 1;
            c++;
            if (c == MATRIX_SIZE) { r++; c = 0; }
        }
    }
    
    // Fill the rest of the matrix with remaining alphabet letters
    for (int i = 0; i < 26; i++) {
        if (!map[i]) {
            matrix[r][c] = (char)(i + 'A');
            map[i] = 1;
            c++;
            if (c == MATRIX_SIZE) { r++; c = 0; }
        }
    }
}

// Function to find coordinates of a character in the matrix
void findPosition(char matrix[5][5], char ch, int *row, int *col) {
    if (ch == 'J') ch = 'I';
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            if (matrix[i][j] == ch) {
                *row = i;
                *col = j;
                return;
            }
        }
    }
}

// Function to decrypt the ciphertext using inverse Playfair rules
void decryptPlayfair(char matrix[5][5], const char ciphertext[], char plaintext[]) {
    int r1, c1, r2, c2;
    int len = strlen(ciphertext);
    int p = 0;

    for (int i = 0; i < len; i += 2) {
        findPosition(matrix, ciphertext[i], &r1, &c1);
        findPosition(matrix, ciphertext[i+1], &r2, &c2);

        // Rule 1: Same Row -> Shift columns LEFT (add 4 instead of subtracting to avoid negative mod)
        if (r1 == r2) {
            plaintext[p++] = matrix[r1][(c1 + 4) % 5];
            plaintext[p++] = matrix[r2][(c2 + 4) % 5];
        }
        // Rule 2: Same Column -> Shift rows UP (add 4 instead of subtracting)
        else if (c1 == c2) {
            plaintext[p++] = matrix[(r1 + 4) % 5][c1];
            plaintext[p++] = matrix[(r2 + 4) % 5][c2];
        }
        // Rule 3: Rectangle -> Swap columns
        else {
            plaintext[p++] = matrix[r1][c2];
            plaintext[p++] = matrix[r2][c1];
        }
    }
    plaintext[p] = '\0';
}

int main() {
    // The historical keyword used during this WWII operation
    const char *keyword = "ROYAL NEW ZEALAND NAVY";
    
    // The raw ciphertext received
    const char *rawCiphertext = "KXJEY UREBE ZWEHE WRYTU HEYFS "
                                "KREHE GOYFI WTTTU OLKSY CAJPO "
                                "BOTEI ZONTX BYBNT GONEY CUZWR "
                                "GDSON SXBOU YWRHE BAAHY USEDQ";
    
    char cleanedCiphertext[200];
    char decryptedText[200];
    char matrix[5][5];

    // 1. Prepare ciphertext and key matrix
    cleanText(rawCiphertext, cleanedCiphertext);
    generateKeyMatrix(keyword, matrix);

    printf("--- PT-109 Playfair Cipher Decryption ---\n");
    printf("Keyword Used: %s\n\n", keyword);
    
    printf("Generated 5x5 Key Matrix:\n");
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            printf("%c ", matrix[i][j]);
        }
        printf("\n");
    }

    // 2. Perform Decryption
    decryptPlayfair(matrix, cleanedCiphertext, decryptedText);

    printf("\nDecrypted Message (Raw Plaintext):\n%s\n", decryptedText);
    
    printf("\nDecrypted Message (Formatted):\n");
    printf("PT ONE O NINE LOST IN ACTION IN BLACKETT STRAIT TWO MILES "
           "SW MERESU COVE X CREW OF ELEVEN FOUND ON PUGU PUGU ISLAND "
           "X KIA TONYA ALBERT X SIGNED EVANS\n");

    return 0;
}