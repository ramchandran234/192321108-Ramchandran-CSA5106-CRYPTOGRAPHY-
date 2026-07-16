#include <stdio.h>
#include <stdint.h>

// A mock 64-bit block cipher encryption function (for demonstration purposes)
uint64_t encrypt_block(uint64_t key, uint64_t plaintext) {
    // Simple mock block cipher: Feistel-like mixing using the key
    uint64_t cipher = plaintext ^ key;
    cipher = (cipher << 13) | (cipher >> 51); // Rotate
    cipher = cipher + 0x9E3779B97F4A7C15ULL;   // Constant mix
    return cipher ^ key;
}

// CBC-MAC implementation for an arbitrary number of blocks
uint64_t cbc_mac(uint64_t key, uint64_t *message, size_t blocks) {
    uint64_t current_state = 0; // Initialize IV to 0 as per standard CBC-MAC
    
    for (size_t i = 0; i < blocks; i++) {
        current_state ^= message[i];
        current_state = encrypt_block(key, current_state);
    }
    
    return current_state;
}

int main() {
    // Secret Key known only to the signer/verification oracle
    uint64_t secret_key = 0xDEADBEEFCAFEBABEULL;
    
    // 1. One-block message X chosen by the adversary
    uint64_t X = 0x0123456789ABCDEFULL;
    
    printf("--- Phase 1: Legitimate MAC Request ---\n");
    printf("Adversary requests MAC for 1-block message X: 0x%016llX\n", X);
    
    // Adversary obtains T = MAC(K, X)
    uint64_t T = cbc_mac(secret_key, &X, 1);
    printf("Oracle returns Tag T:                        0x%016llX\n\n", T);
    
    printf("--- Phase 2: Adversary Forgery ---\n");
    // 2. Adversary constructs the forged 2-block message: X || (X ^ T)
    uint64_t forged_message[2];
    forged_message[0] = X;          // Block 1: X
    forged_message[1] = X ^ T;      // Block 2: X ⊕ T
    
    printf("Adversary constructs 2-block message:\n");
    printf("  Block 1: X         = 0x%016llX\n", forged_message[0]);
    printf("  Block 2: X ^ T     = 0x%016llX\n", forged_message[1]);
    
    // 3. The system validates the forged message using the secret key
    uint64_t forged_mac = cbc_mac(secret_key, forged_message, 2);
    printf("\n--- Phase 3: Verification ---\n");
    printf("Calculated CBC-MAC for forged message:       0x%016llX\n", forged_mac);
    printf("Expected Tag (T from Phase 1):               0x%016llX\n", T);
    
    if (forged_mac == T) {
        printf("\nSUCCESS: The forgery is VALID! The MAC matches perfectly.\n");
    } else {
        printf("\nFAILURE: The MAC does not match.\n");
    }
    
    return 0;
}