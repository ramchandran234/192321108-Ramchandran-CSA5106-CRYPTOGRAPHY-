#include <stdio.h>
#include <ctype.h>
#include <string.h>

// Function to encrypt the text using the polyalphabetic key
void encryptPolyalphabetic(char text[], const char key[]) {
    int textLen = strlen(text);
    int keyLen = strlen(key);
    int keyIndex = 0;

    for (int i = 0; i < textLen; i++) {
        char ch = text[i];

        // Process uppercase letters
        if (isupper(ch)) {
            // Determine shift amount from the current key character (0-25)
            int shift = toupper(key[keyIndex % keyLen]) - 'A';
            text[i] = (ch - 'A' + shift) % 26 + 'A';
            keyIndex++; // Only move to the next key char for alphabetic letters
        }
        // Process lowercase letters
        else if (islower(ch)) {
            int shift = toupper(key[keyIndex % keyLen]) - 'A';
            text[i] = (ch - 'a' + shift) % 26 + 'a';
            keyIndex++;
        }
        // Spaces and punctuation remain unchanged; keyIndex doesn't advance
    }
}

// Function to decrypt the text
void decryptPolyalphabetic(char text[], const char key[]) {
    int textLen = strlen(text);
    int keyLen = strlen(key);
    int keyIndex = 0;

    for (int i = 0; i < textLen; i++) {
        char ch = text[i];

        if (isupper(ch)) {
            int shift = toupper(key[keyIndex % keyLen]) - 'A';
            text[i] = (ch - 'A' - shift + 26) % 26 + 'A';
            keyIndex++;
        }
        else if (islower(ch)) {
            int shift = toupper(key[keyIndex % keyLen]) - 'A';
            text[i] = (ch - 'a' - shift + 26) % 26 + 'a';
            keyIndex++;
        }
    }
}

int main() {
    char text[500];
    char key[100];

    printf("--- Polyalphabetic Cipher (Vigenère) ---\n");
    
    printf("Enter the plaintext message: ");
    fgets(text, sizeof(text), stdin);
    text[strcspn(text, "\n")] = '\0'; // Strip newline

    printf("Enter the keyword: ");
    fgets(key, sizeof(key), stdin);
    key[strcspn(key, "\n")] = '\0'; // Strip newline

    // Validate key to ensure it only contains alphabetic characters
    for (int i = 0; key[i] != '\0'; i++) {
        if (!isalpha(key[i])) {
            printf("Error: Key must contain alphabetic characters only!\n");
            return 1;
        }
    }

    // Encrypting
    encryptPolyalphabetic(text, key);
    printf("\nEncrypted ciphertext: %s\n", text);

    // Decrypting
    decryptPolyalphabetic(text, key);
    printf("Decrypted plaintext:  %s\n", text);

    return 0;
}
