#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BLOCK_SIZE 8  // bytes

// Simple XOR encryption of one block (demo only)
void block_encrypt(const unsigned char *in, unsigned char *out, const unsigned char *key) {
    for (int i = 0; i < BLOCK_SIZE; i++) {
        out[i] = in[i] ^ key[i];
    }
}

void block_decrypt(const unsigned char *in, unsigned char *out, const unsigned char *key) {
    // XOR cipher symmetric
    block_encrypt(in, out, key);
}

// Pad plaintext with '1' bit (0x80) followed by zeros
int pad(unsigned char *data, int len) {
    int pad_len = BLOCK_SIZE - (len % BLOCK_SIZE);
    if (pad_len == 0) pad_len = BLOCK_SIZE; // Always pad a block
    data[len] = 0x80; // 1 bit + 7 zero bits
    for (int i = 1; i < pad_len; i++) {
        data[len + i] = 0x00;
    }
    return len + pad_len;
}

// Remove padding, returns new length or -1 if padding invalid
int unpad(unsigned char *data, int len) {
    if (len == 0 || len % BLOCK_SIZE != 0) return -1;

    int i = len - 1;
    // Remove trailing zeros
    while (i >= 0 && data[i] == 0x00) i--;
    if (i < 0 || data[i] != 0x80) return -1;
    return i;  // new length excluding padding
}

// ECB mode encryption/decryption
void ecb_encrypt(const unsigned char *plaintext, unsigned char *ciphertext, int len, const unsigned char *key) {
    for (int i = 0; i < len; i += BLOCK_SIZE) {
        block_encrypt(plaintext + i, ciphertext + i, key);
    }
}

void ecb_decrypt(const unsigned char *ciphertext, unsigned char *plaintext, int len, const unsigned char *key) {
    for (int i = 0; i < len; i += BLOCK_SIZE) {
        block_decrypt(ciphertext + i, plaintext + i, key);
    }
}

// CBC mode encryption/decryption
void cbc_encrypt(const unsigned char *plaintext, unsigned char *ciphertext, int len, const unsigned char *key, const unsigned char *iv) {
    unsigned char prev_block[BLOCK_SIZE];
    memcpy(prev_block, iv, BLOCK_SIZE);

    for (int i = 0; i < len; i += BLOCK_SIZE) {
        unsigned char xor_block[BLOCK_SIZE];
        for (int j = 0; j < BLOCK_SIZE; j++) {
            xor_block[j] = plaintext[i + j] ^ prev_block[j];
        }
        block_encrypt(xor_block, ciphertext + i, key);
        memcpy(prev_block, ciphertext + i, BLOCK_SIZE);
    }
}

void cbc_decrypt(const unsigned char *ciphertext, unsigned char *plaintext, int len, const unsigned char *key, const unsigned char *iv) {
    unsigned char prev_block[BLOCK_SIZE];
    memcpy(prev_block, iv, BLOCK_SIZE);

    for (int i = 0; i < len; i += BLOCK_SIZE) {
        unsigned char decrypted_block[BLOCK_SIZE];
        block_decrypt(ciphertext + i, decrypted_block, key);
        for (int j = 0; j < BLOCK_SIZE; j++) {
            plaintext[i + j] = decrypted_block[j] ^ prev_block[j];
        }
        memcpy(prev_block, ciphertext + i, BLOCK_SIZE);
    }
}

// CFB mode encryption/decryption
void cfb_encrypt(const unsigned char *plaintext, unsigned char *ciphertext, int len, const unsigned char *key, const unsigned char *iv) {
    unsigned char shift_reg[BLOCK_SIZE];
    memcpy(shift_reg, iv, BLOCK_SIZE);

    for (int i = 0; i < len; i += BLOCK_SIZE) {
        unsigned char encrypted_shift[BLOCK_SIZE];
        block_encrypt(shift_reg, encrypted_shift, key);

        for (int j = 0; j < BLOCK_SIZE; j++) {
            ciphertext[i + j] = plaintext[i + j] ^ encrypted_shift[j];
        }

        memcpy(shift_reg, ciphertext + i, BLOCK_SIZE);
    }
}

void cfb_decrypt(const unsigned char *ciphertext, unsigned char *plaintext, int len, const unsigned char *key, const unsigned char *iv) {
    unsigned char shift_reg[BLOCK_SIZE];
    memcpy(shift_reg, iv, BLOCK_SIZE);

    for (int i = 0; i < len; i += BLOCK_SIZE) {
        unsigned char encrypted_shift[BLOCK_SIZE];
        block_encrypt(shift_reg, encrypted_shift, key);

        for (int j = 0; j < BLOCK_SIZE; j++) {
            plaintext[i + j] = ciphertext[i + j] ^ encrypted_shift[j];
        }

        memcpy(shift_reg, ciphertext + i, BLOCK_SIZE);
    }
}

void print_hex(const unsigned char *data, int len) {
    for (int i = 0; i < len; i++) {
        printf("%02X ", data[i]);
        if ((i + 1) % BLOCK_SIZE == 0) printf("  ");
    }
    printf("\n");
}

int main() {
    unsigned char key[BLOCK_SIZE] = {0x1A, 0x2B, 0x3C, 0x4D, 0x5E, 0x6F, 0x77, 0x88};
    unsigned char iv[BLOCK_SIZE]  = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77};

    unsigned char plaintext[256];
    printf("Enter plaintext (max 255 chars):\n");
    fgets((char*)plaintext, 256, stdin);
    int len = strlen((char*)plaintext);
    if (plaintext[len-1] == '\n') {
        plaintext[len-1] = 0;
        len--;
    }

    // Pad plaintext (always pad at least one block)
    unsigned char padded[256];
    memcpy(padded, plaintext, len);
    int padded_len = pad(padded, len);

    printf("Padded plaintext (%d bytes):\n", padded_len);
    print_hex(padded, padded_len);

    // Buffers for ciphertext and decrypted text
    unsigned char ciphertext[256], decrypted[256];

    // ECB mode
    ecb_encrypt(padded, ciphertext, padded_len, key);
    printf("\nECB Ciphertext:\n");
    print_hex(ciphertext, padded_len);

    ecb_decrypt(ciphertext, decrypted, padded_len, key);
    int decrypted_len = unpad(decrypted, padded_len);
    printf("ECB Decrypted plaintext:\n%.*s\n", decrypted_len, decrypted);

    // CBC mode
    cbc_encrypt(padded, ciphertext, padded_len, key, iv);
    printf("\nCBC Ciphertext:\n");
    print_hex(ciphertext, padded_len);

    cbc_decrypt(ciphertext, decrypted, padded_len, key, iv);
    decrypted_len = unpad(decrypted, padded_len);
    printf("CBC Decrypted plaintext:\n%.*s\n", decrypted_len, decrypted);

    // CFB mode
    cfb_encrypt(padded, ciphertext, padded_len, key, iv);
    printf("\nCFB Ciphertext:\n");
    print_hex(ciphertext, padded_len);

    cfb_decrypt(ciphertext, decrypted, padded_len, key, iv);
    decrypted_len = unpad(decrypted, padded_len);
    printf("CFB Decrypted plaintext:\n%.*s\n", decrypted_len, decrypted);

    return 0;
}

