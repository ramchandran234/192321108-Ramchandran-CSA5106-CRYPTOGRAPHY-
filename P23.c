#include <stdio.h>

// --- S-DES Configuration Tables ---
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

// --- Bit Manipulation Utilities ---
int get_bit(int val, int pos, int total_bits) {
    return (val >> (total_bits - pos)) & 1;
}

int set_bit(int current_val, int bit_to_set, int pos, int total_bits) {
    return current_val | (bit_to_set << (total_bits - pos));
}

int permute(int input, const int *table, int table_size, int input_len) {
    int output = 0;
    for (int i = 0; i < table_size; i++) {
        int bit = get_bit(input, table[i], input_len);
        output = set_bit(output, bit, i + 1, table_size);
    }
    return output;
}

// --- Subkey Generation ---
void generate_subkeys(int key, int *k1, int *k2) {
    int p10_key = permute(key, P10, 10, 10);
    int l = (p10_key >> 5) & 0x1F;
    int r = p10_key & 0x1F;
    
    // Left Shift 1
    l = ((l << 1) & 0x1F) | (l >> 4);
    r = ((r << 1) & 0x1F) | (r >> 4);
    *k1 = permute((l << 5) | r, P8, 8, 10);
    
    // Left Shift 2 more
    l = ((l << 2) & 0x1F) | (l >> 3);
    r = ((r << 2) & 0x1F) | (r >> 3);
    *k2 = permute((l << 5) | r, P8, 8, 10);
}

// --- Core S-DES Functions ---
int round_f(int right_half, int subkey) {
    int ep_out = permute(right_half, EP, 8, 4);
    int xor_out = ep_out ^ subkey;
    
    int l_xor = (xor_out >> 4) & 0x0F;
    int r_xor = xor_out & 0x0F;
    
    int r0 = (get_bit(l_xor, 1, 4) << 1) | get_bit(l_xor, 4, 4);
    int c0 = (get_bit(l_xor, 2, 4) << 1) | get_bit(l_xor, 3, 4);
    int s0_val = S0[r0][c0];
    
    int r1 = (get_bit(r_xor, 1, 4) << 1) | get_bit(r_xor, 4, 4);
    int c1 = (get_bit(r_xor, 2, 4) << 1) | get_bit(r_xor, 3, 4);
    int s1_val = S1[r1][c1];
    
    int combined = (s0_val << 2) | s1_val;
    return permute(combined, P4, 4, 4);
}

int sdes_encrypt_block(int block, int k1, int k2) {
    int ip_out = permute(block, IP, 8, 8);
    int l = (ip_out >> 4) & 0x0F;
    int r = ip_out & 0x0F;
    
    int f1 = round_f(r, k1);
    int l1 = l ^ f1;
    
    int f2 = round_f(l1, k2);
    int l2 = r ^ f2;
    
    int combined = (l2 << 4) | l1;
    return permute(combined, IPI, 8, 8);
}

// --- Presentation Helper ---
void print_binary(int value, int bits) {
    for (int i = 1; i <= bits; i++) {
        printf("%d", get_bit(value, i, bits));
        if (i % 4 == 0 && i != bits) printf(" ");
    }
}

// --- Main Testing Routine ---
int main() {
    int key = 0b0111111101;       // 10-bit key
    int base_counter = 0b00000000; // Starting counter value
    
    // Plaintext split into 3 blocks of 8 bits each
    int pt_blocks[3] = { 0b00000001, 0b00000010, 0b00000100 };
    int ct_blocks[3] = { 0 };
    int dt_blocks[3] = { 0 };

    int k1, k2;
    generate_subkeys(key, &k1, &k2);

    printf("Key:        "); print_binary(key, 10); printf("\n");
    printf("Plaintext:  "); 
    for(int i=0; i<3; i++) { print_binary(pt_blocks[i], 8); printf(" "); }
    printf("\n----------------------------------------------------\n");

    // --- COUNTER MODE ENCRYPTION ---
    for (int i = 0; i < 3; i++) {
        int current_counter = base_counter + i;
        
        // 1. Encrypt the counter value
        int keystream_block = sdes_encrypt_block(current_counter, k1, k2);
        
        // 2. XOR with plaintext to yield ciphertext
        ct_blocks[i] = pt_blocks[i] ^ keystream_block;
    }

    printf("Ciphertext: ");
    for(int i=0; i<3; i++) { print_binary(ct_blocks[i], 8); printf(" "); }
    printf("\n");

    // --- COUNTER MODE DECRYPTION ---
    // Note: Decryption in CTR mode uses the exact same encryption block routine
    for (int i = 0; i < 3; i++) {
        int current_counter = base_counter + i;
        
        // 1. Re-encrypt the counter value to get the identical keystream
        int keystream_block = sdes_encrypt_block(current_counter, k1, k2);
        
        // 2. XOR with ciphertext to yield recovered plaintext
        dt_blocks[i] = ct_blocks[i] ^ keystream_block;
    }

    printf("Decrypted:  ");
    for(int i=0; i<3; i++) { print_binary(dt_blocks[i], 8); printf(" "); }
    printf("\n");

    return 0;
}