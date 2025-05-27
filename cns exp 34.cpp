#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/des.h>

#define BLOCK_SIZE 8

// Add 1-bit + 0 padding to fill up to block size
int pad(unsigned char *input, int input_len, unsigned char **output) {
    int pad_len = BLOCK_SIZE - (input_len % BLOCK_SIZE);
    int total_len = input_len + pad_len;

    *output = (unsigned char *)malloc(total_len);
    memcpy(*output, input, input_len);

    (*output)[input_len] = 0x80; // 1 bit followed by zeros
    for (int i = input_len + 1; i < total_len; i++) {
        (*output)[i] = 0x00;
    }

    return total_len;
}

// Remove 1-bit + 0 padding
int unpad(unsigned char *input, int input_len) {
    for (int i = input_len - 1; i >= 0; i--) {
        if (input[i] == 0x80) {
            return i;  // Length before padding
        }
    }
    return input_len; // No padding found
}

// Helper to print byte arrays
void print_hex(const char *label, unsigned char *data, int len) {
    printf("%s: ", label);
    for (int i = 0; i < len; i++)
        printf("%02X ", data[i]);
    printf("\n");
}

void des_encrypt_decrypt(const char *mode_name, int mode) {
    DES_cblock key = {'s','e','c','r','e','t','k','y'};
    DES_key_schedule schedule;
    DES_set_key_unchecked(&key, &schedule);

    unsigned char iv[BLOCK_SIZE] = {0};
    unsigned char *padded, *ciphertext, *decrypted;
    int padded_len;

    // Example message
    unsigned char message[] = "HelloWorldDESMode";

    padded_len = pad(message, strlen((char *)message), &padded);
    ciphertext = malloc(padded_len);
    decrypted = malloc(padded_len);

    if (mode == DES_ENCRYPT) {
        printf("\n--- %s ENCRYPTION ---\n", mode_name);
    } else {
        printf("\n--- %s DECRYPTION ---\n", mode_name);
    }

    if (strcmp(mode_name, "ECB") == 0) {
        for (int i = 0; i < padded_len; i += BLOCK_SIZE) {
            DES_ecb_encrypt((DES_cblock *)(padded + i), (DES_cblock *)(ciphertext + i), &schedule, DES_ENCRYPT);
        }
    } else if (strcmp(mode_name, "CBC") == 0) {
        DES_ncbc_encrypt(padded, ciphertext, padded_len, &schedule, (DES_cblock *)iv, DES_ENCRYPT);
    } else if (strcmp(mode_name, "CFB") == 0) {
        int num = 0;
        DES_cfb64_encrypt(padded, ciphertext, padded_len, &schedule, (DES_cblock *)iv, &num, DES_ENCRYPT);
    }

    print_hex("Plaintext", padded, padded_len);
    print_hex("Ciphertext", ciphertext, padded_len);

    // Reset IV for decryption
    memset(iv, 0, BLOCK_SIZE);

    if (strcmp(mode_name, "ECB") == 0) {
        for (int i = 0; i < padded_len; i += BLOCK_SIZE) {
            DES_ecb_encrypt((DES_cblock *)(ciphertext + i), (DES_cblock *)(decrypted + i), &schedule, DES_DECRYPT);
        }
    } else if (strcmp(mode_name, "CBC") == 0) {
        DES_ncbc_encrypt(ciphertext, decrypted, padded_len, &schedule, (DES_cblock *)iv, DES_DECRYPT);
    } else if (strcmp(mode_name, "CFB") == 0) {
        int num = 0;
        DES_cfb64_encrypt(ciphertext, decrypted, padded_len, &schedule, (DES_cblock *)iv, &num, DES_DECRYPT);
    }

    int unpadded_len = unpad(decrypted, padded_len);
    decrypted[unpadded_len] = '\0';
    print_hex("Decrypted", decrypted, padded_len);
    printf("Recovered Plaintext: %s\n", decrypted);

    free(padded);
    free(ciphertext);
    free(decrypted);
}

int main() {
    des_encrypt_decrypt("ECB", DES_ENCRYPT);
    des_encrypt_decrypt("CBC", DES_ENCRYPT);
    des_encrypt_decrypt("CFB", DES_ENCRYPT);
    return 0;
}

