#include <stdio.h>
#include <string.h>
#include <ctype.h>
int modInverse(int a) {
    a = a % 26;
    for (int x = 1; x < 26; x++) {
        if ((a * x) % 26 == 1)
            return x;
    }
    return -1; 
}
char encryptChar(char p, int a, int b) {
    if (!isalpha(p)) return p;
    p = toupper(p);
    return ((a * (p - 'A') + b) % 26) + 'A';
}
char decryptChar(char c, int a, int b) {
    if (!isalpha(c)) return c;
    int a_inv = modInverse(a);
    if (a_inv == -1) {
        printf("Error: Invalid 'a', no modular inverse exists.\n");
        return '?';
    }
    c = toupper(c);
    return (a_inv * ((c - 'A') - b + 26)) % 26 + 'A';
}

void encryptText(char* plaintext, char* ciphertext, int a, int b) {
    for (int i = 0; plaintext[i]; i++) {
        ciphertext[i] = encryptChar(plaintext[i], a, b);
    }
    ciphertext[strlen(plaintext)] = '\0';
}

void decryptText(char* ciphertext, char* plaintext, int a, int b) {
    for (int i = 0; ciphertext[i]; i++) {
        plaintext[i] = decryptChar(ciphertext[i], a, b);
    }
    plaintext[strlen(ciphertext)] = '\0';
}

int main() {
    char plaintext[100], ciphertext[100], decrypted[100];
    int a, b;

    printf("Enter the value of a (must be coprime with 26): ");
    scanf("%d", &a);
    printf("Enter the value of b (0-25): ");
    scanf("%d", &b);
    getchar(); c

    if (modInverse(a) == -1) {
        printf("Error: 'a' is not valid. It must be coprime with 26.\n");
        return 1;
    }

    printf("Enter plaintext: ");
    fgets(plaintext, sizeof(plaintext), stdin);
    plaintext[strcspn(plaintext, "\n")] = '\0';

    encryptText(plaintext, ciphertext, a, b);
    decryptText(ciphertext, decrypted, a, b);

    printf("\nEncrypted: %s", ciphertext);
    printf("\nDecrypted: %s\n", decrypted);

    return 0;
}

