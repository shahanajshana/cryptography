#include <stdio.h>
#include <string.h>

#define BLOCK_SIZE 8  // For demonstration, block size = 8 bytes
#define MAX_TEXT 1024

// Simple XOR encryption for demonstration (key is 8 bytes)
void encrypt_ecb(const unsigned char *plaintext, unsigned char *ciphertext, int length, const unsigned char *key) {
    for (int i = 0; i < length; i++) {
        ciphertext[i] = plaintext[i] ^ key[i % BLOCK_SIZE];
    }
}

void decrypt_ecb(const unsigned char *ciphertext, unsigned char *plaintext, int length, const unsigned char *key) {
    // XOR decryption is symmetric to encryption
    encrypt_ecb(ciphertext, plaintext, length, key);
}

void print_hex(const unsigned char *data, int length) {
    for (int i = 0; i < length; i++) {
        printf("%02X ", data[i]);
        if ((i + 1) % BLOCK_SIZE == 0) printf("  ");
    }
    printf("\n");
}

int main() {
    unsigned char key[BLOCK_SIZE] = {0x1A, 0x2B, 0x3C, 0x4D, 0x5E, 0x6F, 0x77, 0x88};

    // Example plaintext (multiple of BLOCK_SIZE)
    unsigned char plaintext[MAX_TEXT] = "This is a secret message that is exactly 48 bytes long!";
    int length = strlen((char *)plaintext);

    // Pad plaintext to multiple of BLOCK_SIZE (simple zero padding)
    int padded_len = length;
    if (padded_len % BLOCK_SIZE != 0) {
        padded_len = ((padded_len / BLOCK_SIZE) + 1) * BLOCK_SIZE;
        for (int i = length; i < padded_len; i++) plaintext[i] = 0;
    }

    unsigned char ciphertext[MAX_TEXT];
    unsigned char decrypted[MAX_TEXT];

    // Encrypt in ECB mode
    encrypt_ecb(plaintext, ciphertext, padded_len, key);

    printf("Plaintext:\n%s\n\n", plaintext);
    printf("Ciphertext (hex):\n");
    print_hex(ciphertext, padded_len);

    // Simulate error in ciphertext block 1 (flip one bit in byte 8)
    ciphertext[8] ^= 0x01;

    // Decrypt ciphertext with error
    decrypt_ecb(ciphertext, decrypted, padded_len, key);

    printf("\nDecrypted plaintext after error in ciphertext block 1:\n%s\n", decrypted);

    return 0;
}

