#include <stdio.h>
#include <ctype.h>
#include <string.h>

// Function to encrypt the text
void encryptMonoalphabetic(char text[], const char key[]) {
    for (int i = 0; text[i] != '\0'; i++) {
        char ch = text[i];

        if (isupper(ch)) {
            // Find the 0-25 index of the uppercase letter
            int index = ch - 'A';
            // Map it to the key (and ensure it's uppercase)
            text[i] = toupper(key[index]);
        } 
        else if (islower(ch)) {
            // Find the 0-25 index of the lowercase letter
            int index = ch - 'a';
            // Map it to the key (and convert to lowercase to preserve case)
            text[i] = tolower(key[index]);
        }
        // Non-alphabetic characters (spaces, punctuation) remain unchanged
    }
}

// Function to decrypt the text
void decryptMonoalphabetic(char text[], const char key[]) {
    for (int i = 0; text[i] != '\0'; i++) {
        char ch = text[i];
        
        if (isupper(ch)) {
            // Search for the position of the character in the key
            for (int j = 0; j < 26; j++) {
                if (toupper(key[j]) == ch) {
                    text[i] = 'A' + j; // Reverse map to standard alphabet
                    break;
                }
            }
        } 
        else if (islower(ch)) {
            // Search for the position of the character in the key
            for (int j = 0; j < 26; j++) {
                if (tolower(key[j]) == ch) {
                    text[i] = 'a' + j; // Reverse map to standard alphabet
                    break;
                }
            }
        }
    }
}

int main() {
    char text[500];
    // The key must be a unique permutation of all 26 letters of the alphabet
    char key[27] = "QWERTYUIOPASDFGHJKLZXCVBNM"; 

    printf("--- Monoalphabetic Substitution Cipher ---\n");
    printf("Using Cipher Key: %s\n\n", key);

    printf("Enter a message to encrypt: ");
    fgets(text, sizeof(text), stdin);

    // Remove newline character if captured by fgets
    text[strcspn(text, "\n")] = '\0';

    // Encrypting
    encryptMonoalphabetic(text, key);
    printf("Encrypted message: %s\n", text);

    // Decrypting
    decryptMonoalphabetic(text, key);
    printf("Decrypted message: %s\n", text);

    return 0;
}