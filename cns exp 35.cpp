#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LEN 256

// Encrypt single character with shift
char encrypt_char(char ch, int key) {
    if (isupper(ch)) {
        return ((ch - 'A' + key) % 26) + 'A';
    }
    return ch;
}

// Decrypt single character with shift
char decrypt_char(char ch, int key) {
    if (isupper(ch)) {
        return ((ch - 'A' - key + 26) % 26) + 'A';
    }
    return ch;
}

int main() {
    char plaintext[MAX_LEN];
    char ciphertext[MAX_LEN];
    char decrypted[MAX_LEN];
    int key[MAX_LEN];

    // Input plaintext
    printf("Enter UPPERCASE plaintext (A–Z only): ");
    fgets(plaintext, MAX_LEN, stdin);
    plaintext[strcspn(plaintext, "\n")] = '\0';

    // Convert to uppercase (if needed)
    for (int i = 0; plaintext[i]; i++) {
        plaintext[i] = toupper(plaintext[i]);
    }

    // Seed RNG
    srand(time(NULL));

    // Encrypt
    int len = strlen(plaintext);
    for (int i = 0; i < len; i++) {
        if (isupper(plaintext[i])) {
            key[i] = rand() % 27;  // 0 to 26
            ciphertext[i] = encrypt_char(plaintext[i], key[i]);
        } else {
            key[i] = 0;
            ciphertext[i] = plaintext[i];
        }
    }
    ciphertext[len] = '\0';

    // Decrypt
    for (int i = 0; i < len; i++) {
        if (isupper(ciphertext[i])) {
            decrypted[i] = decrypt_char(ciphertext[i], key[i]);
        } else {
            decrypted[i] = ciphertext[i];
        }
    }
    decrypted[len] = '\0';

    // Print results
    printf("\nGenerated Key:     ");
    for (int i = 0; i < len; i++) {
        if (isupper(plaintext[i]))
            printf("%2d ", key[i]);
        else
            printf("   ");
    }

    printf("\nPlaintext:         %s\n", plaintext);
    printf("Ciphertext:        %s\n", ciphertext);
    printf("Decrypted Text:    %s\n", decrypted);

    return 0;
}

