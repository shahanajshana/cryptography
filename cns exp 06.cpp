#include <stdio.h>
#include <string.h>
#include <ctype.h>
int modInverse(int a, int m) {
    a = a % m;
    for (int x = 1; x < m; x++) {
        if ((a * x) % m == 1) return x;
    }
    return -1;
}
char decryptChar(char c, int a_inv, int b) {
    if (!isalpha(c)) return c;
    c = toupper(c);
    return (char)(((a_inv * ((c - 'A') - b + 26)) % 26) + 'A');
}
void decryptText(char *cipher, char *plain, int a, int b) {
    int a_inv = modInverse(a, 26);
    if (a_inv == -1) {
        printf("Invalid key. 'a' has no modular inverse.\n");
        return;
    }

    for (int i = 0; cipher[i]; i++) {
        plain[i] = decryptChar(cipher[i], a_inv, b);
    }
    plain[strlen(cipher)] = '\0';
}
int solveKeys(int p1, int c1, int p2, int c2, int *a, int *b) {
    int det = (p1 - p2 + 26) % 26;
    int inv = modInverse(det, 26);
    if (inv == -1) return 0;

    *a = ((c1 - c2 + 26) * inv) % 26;
    *b = (c1 - (*a) * p1 + 26 * 26) % 26;
    return 1;
}

int main() {
    char ciphertext[500], plaintext[500];
    int a, b;

    printf("Enter the ciphertext: ");
    fgets(ciphertext, sizeof(ciphertext), stdin);
    ciphertext[strcspn(ciphertext, "\n")] = '\0';

    int p1 = 4, c1 = 1;
    int p2 = 19, c2 = 20;

    if (!solveKeys(p1, c1, p2, c2, &a, &b)) {
        printf("Cannot break the cipher with given assumptions.\n");
        return 1;
    }

    printf("Assumed key: a = %d, b = %d\n", a, b);
    decryptText(ciphertext, plaintext, a, b);
    printf("Decrypted Text: %s\n", plaintext);

    return 0;
}

