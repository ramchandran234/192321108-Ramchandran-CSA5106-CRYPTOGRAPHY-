#include <stdio.h>
#include <ctype.h>

// Function to encrypt the text
void encryptCaesar(char text[], int k) {
    for (int i = 0; text[i] != '\0'; i++) {
        char ch = text[i];
        
        // Encrypt uppercase letters
        if (isupper(ch)) {
            text[i] = (ch - 'A' + k) % 26 + 'A';
        }
        // Encrypt lowercase letters
        else if (islower(ch)) {
            text[i] = (ch - 'a' + k) % 26 + 'a';
        }
        // Non-alphabetic characters remain unchanged
    }
}

// Function to decrypt the text
void decryptCaesar(char text[], int k) {
    for (int i = 0; text[i] != '\0'; i++) {
        char ch = text[i];
        
        // Decrypt uppercase letters
        if (isupper(ch)) {
            text[i] = (ch - 'A' - k + 26) % 26 + 'A';
        }
        // Decrypt lowercase letters
        else if (islower(ch)) {
            text[i] = (ch - 'a' - k + 26) % 26 + 'a';
        }
    }
}

int main() {
    char text[500];
    int k;

    printf("Enter a message to encrypt: ");
    fgets(text, sizeof(text), stdin);

    printf("Enter key (shift amount 1-25): ");
    scanf("%d", &k);

    // Validate the key range
    if (k < 1 || k > 25) {
        printf("Invalid key! Please run the program again and enter a key between 1 and 25.\n");
        return 1;
    }

    // Encrypting
    encryptCaesar(text, k);
    printf("\nEncrypted message: %s", text);

    // Decrypting back to original
    decryptCaesar(text, k);
    printf("Decrypted message: %s", text);

    return 0;
}