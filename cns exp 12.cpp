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

// Function to calculate the determinant of a 2x2 matrix mod 26
int determinant(int a, int b, int c, int d) {
    return (a * d - b * c) % MOD;
}

// Function to find the modular inverse of a number mod 26
int mod_inverse(int a) {
    for (int i = 1; i < MOD; i++) {
        if ((a * i) % MOD == 1) {
            return i;
        }
    }
    return -1;  // In case no inverse is found
}

// Function to calculate the inverse of a 2x2 matrix mod 26
void inverse_matrix(int key[2][2], int inverse[2][2]) {
    int det = determinant(key[0][0], key[0][1], key[1][0], key[1][1]);
    int inv_det = mod_inverse(det);

    // Compute the inverse matrix
    inverse[0][0] = (inv_det * key[1][1]) % MOD;
    inverse[0][1] = (MOD - inv_det * key[0][1]) % MOD;
    inverse[1][0] = (MOD - inv_det * key[1][0]) % MOD;
    inverse[1][1] = (inv_det * key[0][0]) % MOD;
}

// Function to encrypt a pair of letters
void encrypt_pair(int key[2][2], char a, char b, char *cipher1, char *cipher2) {
    int x1 = letter_to_number(a);
    int x2 = letter_to_number(b);

    *cipher1 = number_to_letter((key[0][0] * x1 + key[0][1] * x2) % MOD);
    *cipher2 = number_to_letter((key[1][0] * x1 + key[1][1] * x2) % MOD);
}

// Function to decrypt a pair of letters
void decrypt_pair(int key[2][2], char a, char b, char *plain1, char *plain2) {
    int x1 = letter_to_number(a);
    int x2 = letter_to_number(b);

    *plain1 = number_to_letter((key[0][0] * x1 + key[0][1] * x2) % MOD);
    *plain2 = number_to_letter((key[1][0] * x1 + key[1][1] * x2) % MOD);
}

// Main function
int main() {
    char plaintext[] = "meetmeattheusualplaceattenratherthaneightoclock";
    char ciphertext[100];
    char decryptedtext[100];
    int key[2][2] = {{9, 4}, {5, 7}};
    int inverse_key[2][2];

    // Inverse the key matrix for decryption
    inverse_matrix(key, inverse_key);

    int len = strlen(plaintext);
    int i = 0;

    // Encrypt the message
    for (i = 0; i < len; i += 2) {
        encrypt_pair(key, plaintext[i], plaintext[i + 1], &ciphertext[i], &ciphertext[i + 1]);
    }
    ciphertext[len] = '\0';  // Null-terminate the ciphertext string

    printf("Ciphertext: %s\n", ciphertext);

    // Decrypt the ciphertext
    for (i = 0; i < len; i += 2) {
        decrypt_pair(inverse_key, ciphertext[i], ciphertext[i + 1], &decryptedtext[i], &decryptedtext[i + 1]);
    }
    decryptedtext[len] = '\0';  // Null-terminate the decrypted string

    printf("Decrypted Text: %s\n", decryptedtext);

    return 0;
}

