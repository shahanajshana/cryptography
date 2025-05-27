#include <stdio.h>
#include <string.h>

// Permutations and S-boxes for S-DES
int P10[] = {3,5,2,7,4,10,1,9,8,6};
int P8[] = {6,3,7,4,8,5,10,9};
int IP[] = {2,6,3,1,4,8,5,7};
int IP_INV[] = {4,1,3,5,7,2,8,6};
int EP[] = {4,1,2,3,2,3,4,1};
int P4[] = {2,4,3,1};

int S0[4][4] = {
    {1,0,3,2},
    {3,2,1,0},
    {0,2,1,3},
    {3,1,3,2}
};

int S1[4][4] = {
    {0,1,2,3},
    {2,0,1,3},
    {3,0,1,0},
    {2,1,0,3}
};

// Helper to get bit (1 or 0) at position pos (1-based)
int get_bit(int val, int pos) {
    return (val >> (pos - 1)) & 1;
}

// Helper to set bit (1 or 0) at position pos (1-based)
int set_bit(int val, int pos, int bit) {
    if(bit) return val | (1 << (pos -1));
    else return val & ~(1 << (pos -1));
}

// Permute bits of input using permutation array, output length = perm_len
int permute(int input, int perm[], int perm_len) {
    int output = 0;
    for (int i = 0; i < perm_len; i++) {
        output <<=1;
        output |= get_bit(input, perm[i]);
    }
    return output;
}

// Left shift n-bit half by 1 (for 5-bit halves)
int left_shift_5(int half) {
    int leftmost = (half >> 4) & 1; // leftmost bit of 5-bit half
    half = ((half << 1) & 0x1F) | leftmost;
    return half;
}

// Generate subkeys K1 and K2 from 10-bit key
void generate_keys(int key, int *K1, int *K2) {
    // P10
    int permuted = permute(key, P10, 10);
    // Split halves
    int left = (permuted >> 5) & 0x1F;  // left 5 bits
    int right = permuted & 0x1F;        // right 5 bits

    // LS-1
    left = left_shift_5(left);
    right = left_shift_5(right);

    int combined = (left << 5) | right;
    *K1 = permute(combined, P8, 8);

    // LS-2
    left = left_shift_5(left_shift_5(left));
    right = left_shift_5(left_shift_5(right));

    combined = (left << 5) | right;
    *K2 = permute(combined, P8, 8);
}

// S-box lookup (4 bits input ? 2 bits output)
int sbox_lookup(int input, int sbox[4][4]) {
    int row = ((input & 0x8) >> 2) | (input & 0x1); // bits 1 and 4
    int col = (input & 0x6) >> 1; // bits 2 and 3
    return sbox[row][col];
}

// fk function for S-DES
int fk(int input, int subkey) {
    int left = (input >> 4) & 0xF;
    int right = input & 0xF;

    // Expand and permute right 4 bits (8 bits)
    int expanded = 0;
    for(int i = 0; i < 8; i++) {
        expanded <<= 1;
        expanded |= (right >> (4 - EP[i])) & 1;
    }

    // XOR with subkey
    expanded ^= subkey;

    // S-boxes
    int left_sbox_input = (expanded >> 4) & 0xF;
    int right_sbox_input = expanded & 0xF;

    int s0_val = sbox_lookup(left_sbox_input, S0);
    int s1_val = sbox_lookup(right_sbox_input, S1);

    int sbox_output = (s0_val << 2) | s1_val;

    // P4 permutation
    int p4_out = 0;
    for(int i = 0; i < 4; i++) {
        p4_out <<= 1;
        p4_out |= (sbox_output >> (4 - P4[i])) & 1;
    }

    return (left ^ p4_out) << 4 | right;
}

// One round of S-DES: switch halves after fk except last round
int sdes_round(int input, int subkey, int switch_halves) {
    int after_fk = fk(input, subkey);
    if(switch_halves) {
        return ((after_fk & 0xF) << 4) | ((after_fk >> 4) & 0xF);
    } else {
        return after_fk;
    }
}

// Encrypt 8-bit block with S-DES
int sdes_encrypt(int plaintext, int K1, int K2) {
    int ip = permute(plaintext, IP, 8);
    int round1 = sdes_round(ip, K1, 1);
    int round2 = sdes_round(round1, K2, 0);
    int ciphertext = permute(round2, IP_INV, 8);
    return ciphertext;
}

// Decrypt 8-bit block with S-DES
int sdes_decrypt(int ciphertext, int K1, int K2) {
    int ip = permute(ciphertext, IP, 8);
    int round1 = sdes_round(ip, K2, 1);
    int round2 = sdes_round(round1, K1, 0);
    int plaintext = permute(round2, IP_INV, 8);
    return plaintext;
}

// XOR two 8-bit values
int xor8(int a, int b) {
    return a ^ b;
}

// Print 8-bit binary
void print_bin8(int val) {
    for(int i=7; i>=0; i--) {
        printf("%d", (val >> i) & 1);
    }
}

// Convert binary string to int
int binstr_to_int(const char *str) {
    int val = 0;
    while(*str) {
        val <<= 1;
        if(*str == '1') val |= 1;
        else if(*str != '0') break; // ignore non-binary chars
        str++;
    }
    return val;
}

int main() {
    // Test inputs from prompt:
    // IV = 10101010 (0xAA)
    // Plaintext = 00000001 00100011 (0x01 0x23)
    // Key = 0111111101 (binary, 10 bits)

    int IV = binstr_to_int("10101010");
    int plaintext_block1 = binstr_to_int("00000001");
    int plaintext_block2 = binstr_to_int("00100011");
    int key = binstr_to_int("0111111101");

    // Generate subkeys
    int K1, K2;
    generate_keys(key, &K1, &K2);

    // CBC encryption
    int c1 = sdes_encrypt(xor8(plaintext_block1, IV), K1, K2);
    int c2 = sdes_encrypt(xor8(plaintext_block2, c1), K1, K2);

    printf("Ciphertext block 1: ");
    print_bin8(c1);
    printf("\nCiphertext block 2: ");
    print_bin8(c2);
    printf("\n");

    // Expected ciphertext: 11110100 00001011
    printf("Expected ciphertext: 11110100 00001011\n\n");

    // CBC decryption
    int p1 = xor8(sdes_decrypt(c1, K1, K2), IV);
    int p2 = xor8(sdes_decrypt(c2, K1, K2), c1);

    printf("Decrypted plaintext block 1: ");
    print_bin8(p1);
    printf("\nDecrypted plaintext block 2: ");
    print_bin8(p2);
    printf("\n");

    return 0;
}

