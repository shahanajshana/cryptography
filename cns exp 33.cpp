#include <stdio.h>
#include <string.h>
#include <openssl/des.h>

int main() {
    // 8-byte plaintext (64 bits)
    unsigned char plaintext[] = "ABCDEFGH";  // Exactly 8 bytes
    unsigned char ciphertext[8];
    unsigned char decryptedtext[8];

    // 8-byte key (only 56 bits used)
    DES_cblock key = { 's','e','c','r','e','t','k','y' }; // 8-byte key
    DES_key_schedule schedule;

    // Set up key schedule
    DES_set_key_unchecked(&key, &schedule);

    // Encrypt
    DES_ecb_encrypt((DES_cblock *)plaintext, (DES_cblock *)ciphertext, &schedule, DES_ENCRYPT);

    printf("Plaintext:  %s\n", plaintext);
    printf("Ciphertext: ");
    for (int i = 0; i < 8; i++) {
        printf("%02X ", ciphertext[i]);
    }
    printf("\n");

    // Decrypt
    DES_ecb_encrypt((DES_cblock *)ciphertext, (DES_cblock *)decryptedtext, &schedule, DES_DECRYPT);

    printf("Decrypted:  %s\n", decryptedtext);

    return 0;
}

