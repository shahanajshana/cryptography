#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Find modular inverse of a under mod m using brute force
int mod_inverse(int a, int m) {
    a = a % m;
    for (int x = 1; x < m; x++) {
        if ((a * x) % m == 1) return x;
    }
    return -1; // No inverse if a and m are not coprime
}

// Check if a and m are coprime
int gcd(int a, int b) {
    return b == 0 ? a : gcd(b, a % b);
}

// Encrypt single character
char affine_encrypt_char(char p, int a, int b) {
    if (isupper(p)) {
        return ((a * (p - 'A') + b) % 26) + 'A';
    }
    return p;
}

// Decrypt single character
char affine_decrypt_char(char c, int a, int b) {
    int a_inv = mod_inverse(a, 26);
    if (a_inv == -1) {
        printf("Error: 'a' value has no modular inverse mod 26. Decryption not possible.\n");
        exit(1);
    }
    if (isupper(c)) {
        return ((a_inv * ((c - 'A' - b + 26)) % 26) + 'A');
    }
    return c;
}

int main() {
    char plaintext[256], ciphertext[256], decrypted[256];
    int a, b;

    printf("Enter UPPERCASE plaintext (A–Z only): ");
    fgets(plaintext, sizeof(plaintext), stdin);
    plaintext[strcspn(plaintext, "\n")] = '\0';

    printf("Enter key 'a' (must be coprime with 26): ");
    scanf("%d", &a);
    printf("Enter key 'b': ");
    scanf("%d", &b);

    if (gcd(a, 26) != 1) {
        printf("Invalid key 'a': not coprime with 26. Choose a different value.\n");
        return 1;
    }

    // Encrypt
    for (int i = 0; i < strlen(plaintext); i++) {
        ciphertext[i] = affine_encrypt_char(plaintext[i], a, b);
    }
    ciphertext[strlen(plaintext)] = '\0';

    // Decrypt
    for (int i = 0; i < strlen(ciphertext); i++) {
        decrypted[i] = affine_decrypt_char(ciphertext[i], a, b);
    }
    decrypted[strlen(ciphertext)] = '\0';

    printf("\nEncrypted Ciphertext: %s\n", ciphertext);
    printf("Decrypted Plaintext:  %s\n", decrypted);

    return 0;
}

