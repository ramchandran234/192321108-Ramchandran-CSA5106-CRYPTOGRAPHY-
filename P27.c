#include <stdio.h>
#include <string.h>

// Modular Exponentiation: Computes (base^exp) % mod safely
long long power(long long base, long long exp, long long mod) {
    long long res = 1;
    base = base % mod;
    while (exp > 0) {
        if (exp % 2 == 1) res = (res * base) % mod;
        base = (base * base) % mod;
        exp /= 2;
    }
    return res;
}

int main() {
    // Bob's Public Key (Using moderately large numbers for demonstration)
    // Even if n were 2048-bits long, the attack works exactly the same way.
    long long n = 35226697; 
    long long e = 7919;     

    // Alice's secret message: "ATTACK"
    char message[] = "ATTACK";
    int msg_len = strlen(message);
    long long intercepted_ciphertext[20];

    printf("=== ALICE SENDS A MESSAGE ===\n");
    printf("Original Text: %s\n", message);
    printf("Intercepted Ciphertext Blocks: ");
    
    for (int i = 0; i < msg_len; i++) {
        int m = message[i] - 'A'; // Convert character to 0-25 integer
        intercepted_ciphertext[i] = power(m, e, n); // Encrypt
        printf("%lld ", intercepted_ciphertext[i]);
    }
    printf("\n\n");

    // --- THE ATTACKER'S SIDE ---
    printf("=== ATTACKER EXECUTES DICTIONARY ATTACK ===\n");
    
    // Step 1: Precompute the lookup table for all 26 possible letters using Bob's public key
    long long codebook[26];
    for (int i = 0; i < 26; i++) {
        codebook[i] = power(i, e, n);
    }
    printf("1. Generated lookup table for all 26 characters instantly.\n");

    // Step 2: Decrypt the intercepted text using simple lookups
    printf("2. Matching ciphertexts against the codebook...\n");
    printf("Decrypted Plaintext: ");
    
    for (int i = 0; i < msg_len; i++) {
        long long current_ct = intercepted_ciphertext[i];
        
        // Look up the ciphertext in our 26-element array
        for (int j = 0; j < 26; j++) {
            if (codebook[j] == current_ct) {
                printf("%c", 'A' + j); // Reconstruct the letter
                break;
            }
        }
    }
    printf("\n");

    return 0;
}