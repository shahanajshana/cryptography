#include <stdio.h>
#include <stdint.h>
#include <string.h>

// S-DES Parameters
#define KEY_SIZE 10
#define SUB_KEY_SIZE 8
#define BLOCK_SIZE 8

// S-Boxes
const uint8_t S0[4][4] = {
    {1, 0, 3, 2},
    {3, 2, 1, 0},
    {0, 2, 1, 3},
    {3, 1, 3, 2}
};

const uint8_t S1[4][4] = {
    {0, 1, 2, 3},
    {2, 0, 1, 3},
    {3, 0, 1, 0},
    {2, 1, 0, 3}
};

// Function Prototypes
uint8_t permute(uint8_t input, const uint8_t *table, uint8_t size);
uint8_t s_box(uint8_t input, const uint8_t sbox[4][4]);
uint8_t fk(uint8_t input, uint8_t subkey);
void generate_subkeys(uint16_t key, uint8_t *subkey1, uint8_t *subkey2);
uint8_t sdes_encrypt(uint8_t plaintext, uint8_t key);
uint8_t sdes_decrypt(uint8_t ciphertext, uint8_t key);
void counter_mode(uint8_t *input, uint8_t *output, size_t size, uint8_t key, uint8_t initial_counter);

// Permutation Tables
const uint8_t P10[] = {3, 5, 2, 7, 4, 10, 1, 9, 8, 6};
const uint8_t P8[] = {6, 3, 7, 4, 8, 5, 10, 9};
const uint8_t IP[] = {2, 6, 3, 1, 4, 8, 5, 7};
const uint8_t IP_INV[] = {4, 1, 3, 5, 7, 2, 8, 6};
const uint8_t EP[] = {4, 1, 2, 3, 2, 3, 4, 1};
const uint8_t P4[] = {2, 4, 3, 1};

int main() {
    uint8_t plaintext[] = {0x01, 0x02, 0x04}; // Binary: 00000001 00000010 00000100
    uint8_t ciphertext[sizeof(plaintext)];
    uint8_t decrypted[sizeof(plaintext)];
    uint8_t key = 0xFD; // Binary: 0111111101
    uint8_t initial_counter = 0x00; // Binary: 00000000

    printf("Original plaintext: ");
    for (size_t i = 0; i < sizeof(plaintext); i++) {
        printf("%02X ", plaintext[i]);
    }
    printf("\n");

    // Encrypt using Counter mode
    counter_mode(plaintext, ciphertext, sizeof(plaintext), key, initial_counter);
    printf("Ciphertext: ");
    for (size_t i = 0; i < sizeof(ciphertext); i++) {
        printf("%02X ", ciphertext[i]);
    }
    printf("\n");

    // Decrypt using Counter mode
    counter_mode(ciphertext, decrypted, sizeof(ciphertext), key, initial_counter);
    printf("Decrypted plaintext: ");
    for (size_t i = 0; i < sizeof(decrypted); i++) {
        printf("%02X ", decrypted[i]);
    }
    printf("\n");

    return 0;
}

// Function Implementations

// Perform permutation based on the given table
uint8_t permute(uint8_t input, const uint8_t *table, uint8_t size) {
    uint8_t output = 0;
    for (uint8_t i = 0; i < size; i++) {
        output <<= 1;
        output |= (input >> (8 - table[i])) & 0x01;
    }
    return output;
}

// S-Box substitution
uint8_t s_box(uint8_t input, const uint8_t sbox[4][4]) {
    uint8_t row = ((input & 0x08) >> 2) | (input & 0x01);
    uint8_t col = (input >> 1) & 0x03;
    return sbox[row][col];
}

// Feistel function (FK)
uint8_t fk(uint8_t input, uint8_t subkey) {
    uint8_t left = (input >> 4) & 0x0F;
    uint8_t right = input & 0x0F;

    uint8_t ep = permute(right, EP, 8);
    uint8_t xor_res = ep ^ subkey;

    uint8_t sbox_out = (s_box((xor_res >> 4) & 0x0F, S0) << 2) | s_box(xor_res & 0x0F, S1);
    uint8_t p4 = permute(sbox_out, P4, 4);

    return ((left ^ p4) << 4) | right;
}

// Generate subkeys
void generate_subkeys(uint16_t key, uint8_t *subkey1, uint8_t *subkey2) {
    uint16_t permuted_key = permute(key, P10, 10);
    uint16_t left = (permuted_key >> 5) & 0x1F;
    uint16_t right = permuted_key & 0x1F;

    left = ((left << 1) | (left >> 4)) & 0x1F;  // Left shift 1
    right = ((right << 1) | (right >> 4)) & 0x1F;

    *subkey1 = permute((left << 5) | right, P8, 8);

    left = ((left << 2) | (left >> 3)) & 0x1F;  // Left shift 2
    right = ((right << 2) | (right >> 3)) & 0x1F;

    *subkey2 = permute((left << 5) | right, P8, 8);
}

// S-DES Encryption
uint8_t sdes_encrypt(uint8_t plaintext, uint8_t key) {
    uint8_t subkey1, subkey2;
    generate_subkeys(key, &subkey1, &subkey2);

    uint8_t ip = permute(plaintext, IP, 8);
    uint8_t fk1 = fk(ip, subkey1);
    uint8_t swapped = (fk1 << 4) | (fk1 >> 4);
    uint8_t fk2 = fk(swapped, subkey2);

    return permute(fk2, IP_INV, 8);
}

// S-DES Decryption
uint8_t sdes_decrypt(uint8_t ciphertext, uint8_t key) {
    uint8_t subkey1, subkey2;
    generate_subkeys(key, &subkey1, &subkey2);

    uint8_t ip = permute(ciphertext, IP, 8);
    uint8_t fk2 = fk(ip, subkey2);
    uint8_t swapped = (fk2 << 4) | (fk2 >> 4);
    uint8_t fk1 = fk(swapped, subkey1);

    return permute(fk1, IP_INV, 8);
}

// Counter Mode Encryption/Decryption
void counter_mode(uint8_t *input, uint8_t *output, size_t size, uint8_t key, uint8_t initial_counter) {
    uint8_t counter = initial_counter;
    for (size_t i = 0; i < size; i++) {
        uint8_t keystream = sdes_encrypt(counter, key);
        output[i] = input[i] ^ keystream;
        counter++;
    }
}

