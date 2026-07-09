#include <stdio.h>
#include <string.h>

// Structure to match multi-byte UTF-8 symbols with English letters
typedef struct {
    const char *symbol;
    char plaintext;
} CipherMap;

int main() {
    // The exact ciphertext provided
    const char *ciphertext = "53‡‡†305))6*;4826)4‡.)4‡);806*;48†8¶60))85;;]8*;:‡*8†83"
                             "(88)5*†;46(;88*96*?;8)*‡(;485);5*†2:*‡(;4956*2(5*—4)8¶8*"
                             ";4069285);)6†8)4‡‡;1(‡9;48081;8:8‡1;48†85;4)485†528806*81"
                             "(‡9;48;(88;4(‡?34;48)4‡;161;:188;‡?;_";

    // Setup substitution dictionary map (with corrected syntax for 'v')
    CipherMap map[] = {
        {"5", 'a'}, {"2", 'b'}, {"1", 'c'}, {"†", 'd'}, {"8", 'e'},
        {"?", 'f'}, {"3", 'g'}, {"4", 'h'}, {"6", 'i'}, {"0", 'l'},
        {"(", 'm'}, {"*", 'n'}, {"‡", 'o'}, {".", 'p'}, {"¶", 'r'},
        {")", 's'}, {";", 't'}, {"9", 'u'}, {"]", 'v'}, {"—", 'w'},
        {"_", 'x'}, {":", 'y'}
    };
    int mapSize = sizeof(map) / sizeof(map[0]);

    printf("--- Original Ciphertext ---\n%s\n\n", ciphertext);
    printf("--- Decrypted Plaintext ---\n");

    int i = 0;
    int len = strlen(ciphertext);

    // Loop through the string, matching variable-length UTF-8 sequences
    while (i < len) {
        int matched = 0;

        // Try matching strings in the map
        for (int j = 0; j < mapSize; j++) {
            int symLen = strlen(map[j].symbol);
            
            // Check if the current slice matches the symbol sequence
            if (strncmp(&ciphertext[i], map[j].symbol, symLen) == 0) {
                putchar(map[j].plaintext);
                i += symLen; // Skip the size of the matched symbol sequence
                matched = 1;
                break;
            }
        }

        // If a character is unrecognized, print it out cleanly
        if (!matched) {
            putchar(ciphertext[i]);
            i++;
        }
    }
    printf("\n");

    return 0;
}