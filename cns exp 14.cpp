#include <stdio.h>
#include <string.h>

#define MOD 26

// Function to convert letter to number (A=0, B=1, ..., Z=25)
int letter_to_number(char c) {
    return c - 'a';
}

// Function to convert number to letter (0=A, 1=B, ..., 25=Z)
char number_to_letter(int num) {
    return num + 'a';
}

// Function to encrypt the plaintext using a key stream
void encrypt_vigenere(char *plaintext, int *key_stream, int key_length, char *ciphertext) {
    int len = strlen(plaintext);
    int i;

    for (i = 0; i < len; i++) {
        // Encrypt only alphabetic characters
        if (plaintext[i] >= 'a' && plaintext[i] <= 'z') {
            int pt_num = letter_to_number(plaintext[i]);
            int key_num = key_stream[i % key_length];
            int cipher_num = (pt_num + key_num) % MOD;
            ciphertext[i] = number_to_letter(cipher_num);
        } else {
            ciphertext[i] = plaintext[i];  // Copy spaces or non-alphabetic characters as is
        }
    }
    ciphertext[len] = '\0';  // Null-terminate the ciphertext
}

int main() {
    char plaintext[] = "send more money";
    int key_stream[] = {9, 0, 1, 7, 23, 15, 21, 14, 11, 11, 2, 8, 9};  // Key stream
    int key_length = sizeof(key_stream) / sizeof(key_stream[0]);
    char ciphertext[100];

    // Encrypt the plaintext using the one-time pad Vigenère cipher
    encrypt_vigenere(plaintext, key_stream, key_length, ciphertext);

    // Print the encrypted ciphertext
    printf("Ciphertext: %s\n", ciphertext);

    return 0;
}

