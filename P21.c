#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BLOCK_SIZE 8  // 8 bytes = 64 bits

// A simple mock block cipher function (Encrypts 1 block using a key)
void block_encrypt(const unsigned char *input, unsigned char *output, const unsigned char *key) {
    for (int i = 0; i < BLOCK_SIZE; i++) {
        output[i] = input[i] ^ key[i];
    }
}

// A simple mock block cipher function (Decrypts 1 block using a key)
void block_decrypt(const unsigned char *input, unsigned char *output, const unsigned char *key) {
    // For XOR, decryption is the same as encryption
    for (int i = 0; i < BLOCK_SIZE; i++) {
        output[i] = input[i] ^ key[i];
    }
}

// Helper to print data in hex
void print_hex(const char *label, const unsigned char *data, size_t len) {
    printf("%s: ", label);
    for (size_t i = 0; i < len; i++) {
        printf("%02X ", data[i]);
    }
    printf("\n");
}

// --- PADDING FUNCTIONS ---

// Implements the "1 bit followed by 0 bits" padding.
// Since we work with bytes, a '1' bit at the start of padding is 0x80 (10000000 binary).
unsigned char* pad_message(const unsigned char *plaintext, size_t orig_len, size_t *padded_len) {
    // Calculate how many padding bytes are needed. 
    // Even if orig_len % BLOCK_SIZE == 0, we add a full dummy block.
    size_t pad_bytes = BLOCK_SIZE - (orig_len % BLOCK_SIZE);
    *padded_len = orig_len + pad_bytes;

    unsigned char *padded = (unsigned char*)malloc(*padded_len);
    memcpy(padded, plaintext, orig_len);

    // Add the 1 bit (0x80) followed by 0s
    padded[orig_len] = 0x80; 
    for (size_t i = orig_len + 1; i < *padded_len; i++) {
        padded[i] = 0x00;
    }
    return padded;
}

// Strips the padding and returns the original length
size_t unpad_message(unsigned char *decrypted, size_t len) {
    // Scan backwards from the end to find the 0x80 byte
    for (size_t i = len - 1; i >= 0; i--) {
        if (decrypted[i] == 0x80) {
            return i; // The new length excluding the padding byte and trailing zeros
        }
    }
    return len; // Fallback (should not happen if correctly padded)
}

// --- CIPHER MODES ---

void encrypt_ECB(const unsigned char *input, unsigned char *output, size_t len, const unsigned char *key) {
    for (size_t i = 0; i < len; i += BLOCK_SIZE) {
        block_encrypt(&input[i], &output[i], key);
    }
}

void decrypt_ECB(const unsigned char *input, unsigned char *output, size_t len, const unsigned char *key) {
    for (size_t i = 0; i < len; i += BLOCK_SIZE) {
        block_decrypt(&input[i], &output[i], key);
    }
}

void encrypt_CBC(const unsigned char *input, unsigned char *output, size_t len, const unsigned char *key, const unsigned char *iv) {
    unsigned char feedback[BLOCK_SIZE];
    memcpy(feedback, iv, BLOCK_SIZE);

    for (size_t i = 0; i < len; i += BLOCK_SIZE) {
        unsigned char block[BLOCK_SIZE];
        for (int j = 0; j < BLOCK_SIZE; j++) {
            block[j] = input[i + j] ^ feedback[j];
        }
        block_encrypt(block, &output[i], key);
        memcpy(feedback, &output[i], BLOCK_SIZE); // Ciphertext feeds into next block
    }
}

void decrypt_CBC(const unsigned char *input, unsigned char *output, size_t len, const unsigned char *key, const unsigned char *iv) {
    unsigned char feedback[BLOCK_SIZE];
    memcpy(feedback, iv, BLOCK_SIZE);

    for (size_t i = 0; i < len; i += BLOCK_SIZE) {
        unsigned char decrypted_block[BLOCK_SIZE];
        block_decrypt(&input[i], decrypted_block, key);
        
        for (int j = 0; j < BLOCK_SIZE; j++) {
            output[i + j] = decrypted_block[j] ^ feedback[j];
        }
        memcpy(feedback, &input[i], BLOCK_SIZE);
    }
}

// Full-block CFB mode implementation (Segment size = BLOCK_SIZE)
void encrypt_CFB(const unsigned char *input, unsigned char *output, size_t len, const unsigned char *key, const unsigned char *iv) {
    unsigned char shift_reg[BLOCK_SIZE];
    memcpy(shift_reg, iv, BLOCK_SIZE);

    for (size_t i = 0; i < len; i += BLOCK_SIZE) {
        unsigned char cipher_out[BLOCK_SIZE];
        block_encrypt(shift_reg, cipher_out, key); // Encrypt the shift register/IV

        for (int j = 0; j < BLOCK_SIZE; j++) {
            output[i + j] = input[i + j] ^ cipher_out[j];
        }
        memcpy(shift_reg, &output[i], BLOCK_SIZE); // Ciphertext becomes next input
    }
}

void decrypt_CFB(const unsigned char *input, unsigned char *output, size_t len, const unsigned char *key, const unsigned char *iv) {
    unsigned char shift_reg[BLOCK_SIZE];
    memcpy(shift_reg, iv, BLOCK_SIZE);

    for (size_t i = 0; i < len; i += BLOCK_SIZE) {
        unsigned char cipher_out[BLOCK_SIZE];
        block_encrypt(shift_reg, cipher_out, key); // CFB decryption uses block_encrypt!

        for (int j = 0; j < BLOCK_SIZE; j++) {
            output[i + j] = input[i + j] ^ cipher_out[j];
        }
        memcpy(shift_reg, &input[i], BLOCK_SIZE);
    }
}

// --- MAIN PIPELINE ---

void run_demo(const unsigned char *plaintext, size_t orig_len, const unsigned char *key, const unsigned char *iv) {
    size_t padded_len;
    unsigned char *padded_pt = pad_message(plaintext, orig_len, &padded_len);
    
    unsigned char *ciphertext = (unsigned char*)malloc(padded_len);
    unsigned char *decrypted  = (unsigned char*)malloc(padded_len);

    printf("Original Length: %zu bytes. Padded Length: %zu bytes.\n", orig_len, padded_len);
    print_hex("Padded Plaintext", padded_pt, padded_len);
    printf("------------------------------------------------------------\n");

    // 1. ECB Test
    encrypt_ECB(padded_pt, ciphertext, padded_len, key);
    decrypt_ECB(ciphertext, decrypted, padded_len, key);
    size_t ecb_len = unpad_message(decrypted, padded_len);
    printf("[ECB] "); print_hex("Ciphertext", ciphertext, padded_len);
    printf("[ECB] Decrypted String: %.*s\n\n", (int)ecb_len, decrypted);

    // 2. CBC Test
    encrypt_CBC(padded_pt, ciphertext, padded_len, key, iv);
    decrypt_CBC(ciphertext, decrypted, padded_len, key, iv);
    size_t cbc_len = unpad_message(decrypted, padded_len);
    printf("[CBC] "); print_hex("Ciphertext", ciphertext, padded_len);
    printf("[CBC] Decrypted String: %.*s\n\n", (int)cbc_len, decrypted);

    // 3. CFB Test
    encrypt_CFB(padded_pt, ciphertext, padded_len, key, iv);
    decrypt_CFB(ciphertext, decrypted, padded_len, key, iv);
    size_t cfb_len = unpad_message(decrypted, padded_len);
    printf("[CFB] "); print_hex("Ciphertext", ciphertext, padded_len);
    printf("[CFB] Decrypted String: %.*s\n", (int)cfb_len, decrypted);

    free(padded_pt);
    free(ciphertext);
    free(decrypted);
}

int main() {
    unsigned char key[BLOCK_SIZE] = {'m', 'y', 's', 'e', 'c', 'r', 'e', 't'};
    unsigned char iv[BLOCK_SIZE]  = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};

    // Test 1: Message perfectly multiple of BLOCK_SIZE (8 bytes) -> Forces a dummy block
    printf("=== TEST 1: Exactly 8 Bytes (Aligned) ===\n");
    unsigned char pt1[] = "12345678"; 
    run_demo(pt1, 8, key, iv);

    printf("\n============================================================\n\n");

    // Test 2: Message not a multiple of BLOCK_SIZE (11 bytes)
    printf("=== TEST 2: 11 Bytes (Unaligned) ===\n");
    unsigned char pt2[] = "Hello World"; 
    run_demo(pt2, 11, key, iv);

    return 0;
}