#include <stdio.h>
#include <string.h>

// Dummy block cipher: XOR with key
void encryptBlock(unsigned char *block, unsigned char *key) {
    for (int i=0;i<8;i++) block[i]^=key[i];
}
void decryptBlock(unsigned char *block, unsigned char *key) {
    for (int i=0;i<8;i++) block[i]^=key[i];
}

// ECB mode: each block independent
void ECB_decrypt(unsigned char *cipher, int len, unsigned char *key, unsigned char *plain) {
    for (int i=0;i<len;i+=8) {
        memcpy(&plain[i],&cipher[i],8);
        decryptBlock(&plain[i],key);
    }
}

// CBC mode: each block depends on previous ciphertext
void CBC_decrypt(unsigned char *cipher, int len, unsigned char *key, unsigned char *iv, unsigned char *plain) {
    unsigned char prev[8];
    memcpy(prev,iv,8);
    for (int i=0;i<len;i+=8) {
        unsigned char block[8];
        memcpy(block,&cipher[i],8);
        decryptBlock(block,key);
        for (int j=0;j<8;j++) plain[i+j]=block[j]^prev[j];
        memcpy(prev,&cipher[i],8);
    }
}

int main() {
    unsigned char key[8]={1,2,3,4,5,6,7,8};
    unsigned char iv[8]={0,0,0,0,0,0,0,0};
    unsigned char plaintext[16]="ABCDEFGH1234567";
    unsigned char ciphertext[16], decrypted[16];

    // Encrypt in CBC for demo
    unsigned char prev[8]; memcpy(prev,iv,8);
    for (int i=0;i<16;i+=8) {
        unsigned char block[8]; memcpy(block,&plaintext[i],8);
        for (int j=0;j<8;j++) block[j]^=prev[j];
        encryptBlock(block,key);
        memcpy(&ciphertext[i],block,8);
        memcpy(prev,block,8);
    }

    // Introduce error in C1
    ciphertext[0]^=0xFF;

    CBC_decrypt(ciphertext,16,key,iv,decrypted);

    printf("Decrypted with error in C1: %.*s\n",16,decrypted);
    return 0;
}
