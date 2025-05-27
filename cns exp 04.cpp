#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Function to convert string to uppercase
void toUpperCase(char* str) {
    for (int i = 0; str[i]; i++) {
        str[i] = toupper(str[i]);
    }
}

// Function to encrypt using Vigenère Cipher
void encrypt(char* plaintext, char* key, char* ciphertext) {
    int i, j = 0;
    int keyLen = strlen(key);

    for (i = 0; plaintext[i]; i++) {
        if (isalpha(plaintext[i])) {
            char p = toupper(plaintext[i]);
            char k = key[j % keyLen];

            ciphertext[i] = ((p - 'A') + (k - 'A')) % 26 + 'A';
            j++;
        } else {
            ciphertext[i] = plaintext[i]; // Keep non-alphabetic characters as-is
        }
    }
    ciphertext[i] = '\0';
}

// Function to decrypt using Vigenère Cipher
void decrypt(char* ciphertext, char* key, char* plaintext) {
    int i, j = 0;
    int keyLen = strlen(key);

    for (i = 0; ciphertext[i]; i++) {
        if (isalpha(ciphertext[i])) {
            char c = toupper(ciphertext[i]);
            char k = key[j % keyLen];

            plaintext[i] = ((c - k + 26) % 26) + 'A';
            j++;
        } else {
            plaintext[i] = ciphertext[i];
        }
    }
    plaintext[i] = '\0';
}

int main() {
    char plaintext[100], key[100], ciphertext[100], decrypted[100];

    printf("Enter the plaintext: ");
    fgets(plaintext, sizeof(plaintext), stdin);
    plaintext[strcspn(plaintext, "\n")] = '\0'; // Remove trailing newline

    printf("Enter the key: ");
    fgets(key, sizeof(key), stdin);
    key[strcspn(key, "\n")] = '\0';

    toUpperCase(key);

    encrypt(plaintext, key, ciphertext);
    decrypt(ciphertext, key, decrypted);

    printf("\nEncrypted Ciphertext: %s", ciphertext);
    printf("\nDecrypted Plaintext:  %s\n", decrypted);

    return 0;
}

