#include <stdio.h>
#include <string.h>
#include <stdint.h>

// Simulate a block size of 16 bytes (128 bits)
#define BLOCK_SIZE 16

// Dummy encryption function (e.g., AES block cipher simulation)
void dummy_encrypt(uint8_t *block, uint8_t *key) {
    for (int i = 0; i < BLOCK_SIZE; i++) {
        block[i] ^= key[i]; // Simple XOR for demonstration purposes
    }
}

// CBC-MAC computation for a single-block message
void cbc_mac_one_block(uint8_t *key, uint8_t *message, uint8_t *mac) {
    memcpy(mac, message, BLOCK_SIZE); // Copy message to MAC buffer
    dummy_encrypt(mac, key);         // Encrypt the block
}

// CBC-MAC computation for a two-block message
void cbc_mac_two_blocks(uint8_t *key, uint8_t *block1, uint8_t *block2, uint8_t *mac) {
    uint8_t intermediate[BLOCK_SIZE];

    // Compute CBC-MAC for the first block
    memcpy(intermediate, block1, BLOCK_SIZE);
    dummy_encrypt(intermediate, key);

    // XOR the second block with the intermediate result and encrypt
    for (int i = 0; i < BLOCK_SIZE; i++) {
        intermediate[i] ^= block2[i];
    }
    dummy_encrypt(intermediate, key);

    // Output the final MAC
    memcpy(mac, intermediate, BLOCK_SIZE);
}

int main() {
    uint8_t key[BLOCK_SIZE] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
                               0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10};

    uint8_t message[BLOCK_SIZE] = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22,
                                   0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0x00};

    uint8_t mac[BLOCK_SIZE];
    uint8_t forged_mac[BLOCK_SIZE];
    uint8_t second_block[BLOCK_SIZE];

    printf("Computing CBC-MAC for one-block message...\n");
    cbc_mac_one_block(key, message, mac);

    printf("MAC for one-block message: ");
    for (int i = 0; i < BLOCK_SIZE; i++) {
        printf("%02X ", mac[i]);
    }
    printf("\n");

    // Adversary computes the second block as X ? T
    for (int i = 0; i < BLOCK_SIZE; i++) {
        second_block[i] = message[i] ^ mac[i];
    }

    printf("Second block (X ? T): ");
    for (int i = 0; i < BLOCK_SIZE; i++) {
        printf("%02X ", second_block[i]);
    }
    printf("\n");

    // Compute CBC-MAC for the forged two-block message
    printf("Computing CBC-MAC for forged two-block message (X || (X ? T))...\n");
    cbc_mac_two_blocks(key, message, second_block, forged_mac);

    printf("MAC for forged two-block message: ");
    for (int i = 0; i < BLOCK_SIZE; i++) {
        printf("%02X ", forged_mac[i]);
    }
    printf("\n");

    // Check if the adversary succeeded
    if (memcmp(mac, forged_mac, BLOCK_SIZE) == 0) {
        printf("Adversary successfully forged the MAC for a two-block message.\n");
    } else {
        printf("Adversary failed to forge the MAC.\n");
    }

    return 0;
}

