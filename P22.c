#include <stdio.h>

// --- S-DES Configuration and Lookups ---
const int P10[] = {3, 5, 2, 7, 4, 10, 1, 9, 8, 6};
const int P8[]  = {6, 3, 7, 4, 8, 5, 10, 9};
const int IP[]  = {2, 6, 3, 1, 4, 8, 5, 7};
const int IPI[] = {4, 1, 3, 5, 7, 2, 8, 6};
const int EP[]  = {4, 1, 2, 3, 2, 3, 4, 1};
const int P4[]  = {2, 4, 3, 1};

const int S0[4][4] = {
    {1, 0, 3, 2},
    {3, 2, 1, 0},
    {0, 2, 1, 3},
    {3, 1, 3, 2}
};

const int S1[4][4] = {
    {0, 1, 2, 3},
    {2, 0, 1, 3},
    {3, 0, 1, 0},
    {2, 1, 0, 3}
};

// Helper function to extract bit at 1-based position from right (LSB = pos 1)
int get_bit(int val, int pos, int total_bits) {
    return (val >> (total_bits - pos)) & 1;
}

// Helper to set a specific bit in an output container
int set_bit(int current_val, int bit_to_set, int pos, int total_bits) {
    return current_val | (bit_to_set << (total_bits - pos));
}

// General permutation engine
int permute(int input, const int *table, int table_size, int input_len) {
    int output = 0;
    for (int i = 0; i < table_size; i++) {
        int bit = get_bit(input, table[i], input_len);
        output = set_bit(output, bit, i + 1, table_size);
    }
    return output;
}

// Generate K1 and K2 subkeys from 10-bit raw key
void generate_subkeys(int key, int *k1, int *k2) {
    int p10_key = permute(key, P10, 10, 10);
    
    int l = (p10_key >> 5) & 0x1F;
    int r = p10_key & 0x1F;
    
    // Left Shift 1 (Circular)
    l = ((l << 1) & 0x1F) | (l >> 4);
    r = ((r << 1) & 0x1F) | (r >> 4);
    
    *k1 = permute((l << 5) | r, P8, 8, 10);
    
    // Left Shift 2 more (Circular)
    l = ((l << 2) & 0x1F) | (l >> 3);
    r = ((r << 2) & 0x1F) | (r >> 3);
    
    *k2 = permute((l << 5) | r, P8, 8, 10);
}

// S-DES Round Function F
int round_f(int right_half, int subkey) {
    int ep_out = permute(right_half, EP, 8, 4);
    int xor_out = ep_out ^ subkey;
    
    int l_xor = (xor_out >> 4) & 0x0F;
    int r_xor = xor_out & 0x0F;
    
    // S0 lookup
    int r0 = (get_bit(l_xor, 1, 4) << 1) | get_bit(l_xor, 4, 4);
    int c0 = (get_bit(l_xor, 2, 4) << 1) | get_bit(l_xor, 3, 4);
    int s0_val = S0[r0][c0];
    
    // S1 lookup
    int r1 = (get_bit(r_xor, 1, 4) << 1) | get_bit(r_xor, 4, 4);
    int c1 = (get_bit(r_xor, 2, 4) << 1) | get_bit(r_xor, 3, 4);
    int s1_val = S1[r1][c1];
    
    int combined = (s0_val << 2) | s1_val;
    return permute(combined, P4, 4, 4);
}

// Core Feistel Mapping structure block operations
int sdes_encrypt_block(int block, int k1, int k2) {
    int ip_out = permute(block, IP, 8, 8);
    int l = (ip_out >> 4) & 0x0F;
    int r = ip_out & 0x0F;
    
    // Round 1
    int f1 = round_f(r, k1);
    int l1 = l ^ f1;
    
    // Swap halves (r becomes left, l1 becomes right)
    // Round 2
    int f2 = round_f(l1, k2);
    int l2 = r ^ f2;
    
    int combined = (l2 << 4) | l1;
    return permute(combined, IPI, 8, 8);
}

int sdes_decrypt_block(int block, int k1, int k2) {
    // Decryption uses subkeys in reverse order (k2 then k1)
    int ip_out = permute(block, IP, 8, 8);
    int l = (ip_out >> 4) & 0x0F;
    int r = ip_out & 0x0F;
    
    int f1 = round_f(r, k2);
    int l1 = l ^ f1;
    
    int f2 = round_f(l1, k1);
    int l2 = r ^ f2;
    
    int combined = (l2 << 4) | l1;
    return permute(combined, IPI, 8, 8);
}

// Helper function to cleanly print numbers in binary format
void print_binary(int value, int bits) {
    for (int i = 1; i <= bits; i++) {
        printf("%d", get_bit(value, i, bits));
        if (i % 4 == 0 && i != bits) printf(" ");
    }
}

// --- MAIN IMPLEMENTATION ---
int main() {
    // S-DES components from the specification:
    int iv       = 0b10101010; // Binary 1010 1010 (8 bits)
    int key      = 0b0111111101; // Binary 01111 11101 (10 bits)
    
    // Total 16 bits input split into two 8-bit blocks
    int pt_blocks[2] = { 0b00000001, 0b00100011 }; // 0000 0001 and 0010 0011
    int ct_blocks[2] = { 0 };
    int dt_blocks[2] = { 0 };

    int k1, k2;
    generate_subkeys(key, &k1, &k2);

    printf("Key:       "); print_binary(key, 10); printf("\n");
    printf("IV:        "); print_binary(iv, 8); printf("\n");
    printf("Plaintext: "); print_binary(pt_blocks[0], 8); printf(" "); print_binary(pt_blocks[1], 8); printf("\n");
    printf("-------------------------------------------\n");

    // --- CBC ENCRYPTION ---
    int previous_cipher = iv;
    for (int i = 0; i < 2; i++) {
        // XOR current block with the previous ciphertext (or IV for block 0)
        int xor_block = pt_blocks[i] ^ previous_cipher;
        ct_blocks[i] = sdes_encrypt_block(xor_block, k1, k2);
        previous_cipher = ct_blocks[i];
    }

    printf("Ciphertext: "); print_binary(ct_blocks[0], 8); printf(" "); print_binary(ct_blocks[1], 8); printf("\n");

    // --- CBC DECRYPTION ---
    previous_cipher = iv;
    for (int i = 0; i < 2; i++) {
        int decrypted_raw = sdes_decrypt_block(ct_blocks[i], k1, k2);
        // XOR raw decryption with previous ciphertext block to uncover plaintext
        dt_blocks[i] = decrypted_raw ^ previous_cipher;
        previous_cipher = ct_blocks[i];
    }

    printf("Decrypted:  "); print_binary(dt_blocks[0], 8); printf(" "); print_binary(dt_blocks[1], 8); printf("\n");

    return 0;
}