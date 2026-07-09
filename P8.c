#include <stdio.h>
#include <ctype.h>
#include <string.h>

// Function to generate the custom cipher alphabet mapping using a keyword
void generateCipherAlphabet(const char keyword[], char cipherAlphabet[]) {
    int present[26] = {0}; // Track letters already added to the map
    int index = 0;

    // 1. First, insert unique letters from the keyword
    for (int i = 0; keyword[i] != '\0'; i++) {
        if (isalpha(keyword[i])) {
            char ch = toupper(keyword[i]);
            int alphaIdx = ch - 'A';
            
            if (!present[alphaIdx]) {
                cipherAlphabet[index++] = ch;
                present[alphaIdx] = 1;
            }
        }
    }

    // 2. Fill the remaining spots with the unused alphabet letters
    for (int i = 0; i < 26; i++) {
        if (!present[i]) {
            cipherAlphabet[index++] = (char)('A' + i);
        }
    }
    cipherAlphabet[26] = '\0'; // Null-terminate the alphabet string
}

// Function to encrypt the plaintext using the generated mapping
void encrypt(char text[], const char cipherAlphabet[]) {
    for (int i = 0; text[i] != '\0'; i++) {
        char ch = text[i];
        if (isupper(ch)) {
            text[i] = cipherAlphabet[ch - 'A'];
        } else if (islower(ch)) {
            text[i] = tolower(cipherAlphabet[ch - 'a']);
        }
    }
}

// Function to decrypt the ciphertext by reversing the mapping
void decrypt(char text[], const char cipherAlphabet[]) {
    for (int i = 0; text[i] != '\0'; i++) {
        char ch = text[i];
        if (isupper(ch)) {
            // Find where 'ch' sits in our custom cipher mapping
            for (int j = 0; j < 26; j++) {
                if (cipherAlphabet[j] == ch) {
                    text[i] = 'A' + j;
                    break;
                }
            }
        } else if (islower(ch)) {
            char upperCh = toupper(ch);
            for (int j = 0; j < 26; j++) {
                if (cipherAlphabet[j] == upperCh) {
                    text[i] = 'a' + j;
                    break;
                }
            }
        }
    }
}

int main() {
    char keyword[100];
    char text[500];
    char cipherAlphabet[27];

    printf("--- Keyword Monoalphabetic Cipher ---\n");
    
    printf("Enter the keyword (e.g., CIPHER): ");
    scanf("%s", keyword);
    getchar(); // Clear newline buffer left by scanf

    // Generate substitution alphabet sequence
    generateCipherAlphabet(keyword, cipherAlphabet);

    // Print mapping layout for confirmation
    printf("\nPlaintext Alphabet:  ABCDEFGHIJKLMNOPQRSTUVWXYZ\n");
    printf("Ciphertext Alphabet: %s\n\n", cipherAlphabet);

    printf("Enter a message to encrypt: ");
    fgets(text, sizeof(text), stdin);
    text[strcspn(text, "\n")] = '\0'; // Strip trailing newline

    // Encrypting
    encrypt(text, cipherAlphabet);
    printf("Encrypted message:   %s\n", text);

    // Decrypting
    decrypt(text, cipherAlphabet);
    printf("Decrypted message:   %s\n", text);

    return 0;
}