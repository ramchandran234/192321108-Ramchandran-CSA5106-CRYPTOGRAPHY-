#include <stdio.h>
#include <string.h>

// Stub for 3DES encryption of one 64-bit block
void tripleDES_encryptBlock(unsigned char *block, unsigned char *key) {
    // Apply DES three times: Encrypt with key1, Decrypt with key2, Encrypt with key3
    // For simplicity, assume key contains concatenated key1,key2,key3
}

// Stub for 3DES decryption of one 64-bit block
void tripleDES_decryptBlock(unsigned char *block, unsigned char *key) {
    // Reverse order: Decrypt with key3, Encrypt with key2, Decrypt with key1
}

void CBC_encrypt(unsigned char *plaintext, int len, unsigned char *key, unsigned char *iv, unsigned char *ciphertext) {
    unsigned char prev[8];
    memcpy(prev, iv, 8);
    for (int i=0; i<len; i+=8) {
        unsigned char block[8];
        for (int j=0;j<8;j++) block[j]=plaintext[i+j]^prev[j]; // XOR with IV or previous ciphertext
        tripleDES_encryptBlock(block,key);
        memcpy(ciphertext+i,block,8);
        memcpy(prev,block,8);
    }
}

void CBC_decrypt(unsigned char *ciphertext, int len, unsigned char *key, unsigned char *iv, unsigned char *plaintext) {
    unsigned char prev[8];
    memcpy(prev, iv, 8);
    for (int i=0; i<len; i+=8) {
        unsigned char block[8];
        memcpy(block,ciphertext+i,8);
        tripleDES_decryptBlock(block,key);
        for (int j=0;j<8;j++) plaintext[i+j]=block[j]^prev[j];
        memcpy(prev,ciphertext+i,8);
    }
}

int main() {
    unsigned char key[24] = { /* 3DES key: 3 x 8 bytes */ };
    unsigned char iv[8] = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07};
    unsigned char plaintext[16] = "HELLOWORLD12345"; // 16 bytes (2 blocks)
    unsigned char ciphertext[16], decrypted[16];

    CBC_encrypt(plaintext,16,key,iv,ciphertext);
    CBC_decrypt(ciphertext,16,key,iv,decrypted);

    printf("Plaintext:  %s\n", plaintext);
    printf("Ciphertext: ");
    for (int i=0;i<16;i++) printf("%02X", ciphertext[i]);
    printf("\nDecrypted:  %s\n", decrypted);

    return 0;
}
