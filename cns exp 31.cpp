#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <openssl/aes.h>

#define BLOCK_SIZE 16 // 128-bit AES block size

// XOR the last byte with Rb if MSB was 1
void generate_subkeys(uint8_t *key, uint8_t *K1, uint8_t *K2) {
    uint8_t L[BLOCK_SIZE] = {0};
    uint8_t Rb = 0x87;

    AES_KEY aes_key;
    AES_set_encrypt_key(key, 128, &aes_key);

    uint8_t zero_block[BLOCK_SIZE] = {0};
    AES_encrypt(zero_block, L, &aes_key);

    // Generate K1
    int msb_L = (L[0] & 0x80) != 0;
    for (int i = 0; i < BLOCK_SIZE; i++) {
        K1[i] = (L[i] << 1);
        if (i < BLOCK_SIZE - 1 && (L[i + 1] & 0x80)) {
            K1[i] |= 0x01;
        }
    }
    if (msb_L) {
        K1[BLOCK_SIZE - 1] ^= Rb;
    }

    // Generate K2
    int msb_K1 = (K1[0] & 0x80) != 0;
    for (int i = 0; i < BLOCK_SIZE; i++) {
        K2[i] = (K1[i] << 1);
        if (i < BLOCK_SIZE - 1 && (K1[i + 1] & 0x80)) {
            K2[i] |= 0x01;
        }
    }
    if (msb_K1) {
        K2[BLOCK_SIZE - 1] ^= Rb;
    }
}

void print_block(const char *label, uint8_t *block) {
    printf("%s: ", label);
    for (int i = 0; i < BLOCK_SIZE; i++) {
        printf("%02X", block[i]);
    }
    printf("\n");
}

int main() {
    uint8_t key[16] = {
        0x2b, 0x7e, 0x15, 0x16,
        0x28, 0xae, 0xd2, 0xa6,
        0xab, 0xf7, 0x15, 0x88,
        0x09, 0xcf, 0x4f, 0x3c
    };

    uint8_t K1[BLOCK_SIZE], K2[BLOCK_SIZE];

    generate_subkeys(key, K1, K2);

    print_block("K1", K1);
    print_block("K2", K2);

    return 0;
}

