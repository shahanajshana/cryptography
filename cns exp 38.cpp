#include <stdio.h>
#include <stdlib.h>

#define MOD 26

// Function to calculate determinant of 2x2 matrix
int determinant(int key[2][2]) {
    return (key[0][0]*key[1][1] - key[0][1]*key[1][0]) % MOD;
}

// Function to calculate modular inverse of a mod m
int mod_inverse(int a, int m) {
    a = a % m;
    for (int x = 1; x < m; x++) {
        if ((a * x) % m == 1)
            return x;
    }
    return -1; // No inverse exists
}

// Function to find inverse of 2x2 matrix mod 26
int inverse_key(int key[2][2], int inverse[2][2]) {
    int det = determinant(key);
    int det_inv = mod_inverse(det, MOD);
    if (det_inv == -1) {
        return 0; // Inverse doesn't exist
    }

    inverse[0][0] =  key[1][1] * det_inv % MOD;
    inverse[0][1] = -key[0][1] * det_inv % MOD;
    inverse[1][0] = -key[1][0] * det_inv % MOD;
    inverse[1][1] =  key[0][0] * det_inv % MOD;

    // Ensure all values are positive
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++)
            inverse[i][j] = (inverse[i][j] + MOD) % MOD;

    return 1;
}

// Encrypt 2-letter block using Hill cipher
void encrypt_block(char *block, int key[2][2], char *out) {
    int p[2] = { block[0] - 'A', block[1] - 'A' };
    out[0] = (key[0][0]*p[0] + key[0][1]*p[1]) % MOD + 'A';
    out[1] = (key[1][0]*p[0] + key[1][1]*p[1]) % MOD + 'A';
}

// Decrypt 2-letter block using Hill cipher
void decrypt_block(char *block, int key[2][2], char *out) {
    int c[2] = { block[0] - 'A', block[1] - 'A' };
    out[0] = (key[0][0]*c[0] + key[0][1]*c[1]) % MOD + 'A';
    out[1] = (key[1][0]*c[0] + key[1][1]*c[1]) % MOD + 'A';
}

// Simulate known plaintext attack (given two plaintext-ciphertext pairs)
void known_plaintext_attack(char *pt, char *ct, int recovered_key[2][2]) {
    int P[2][2], C[2][2];

    // Build matrices from chars
    for (int i = 0; i < 2; i++) {
        P[0][i] = pt[i] - 'A';
        P[1][i] = pt[i + 2] - 'A';
        C[0][i] = ct[i] - 'A';
        C[1][i] = ct[i + 2] - 'A';
    }

    // Inverse of P
    int P_inv[2][2];
    if (!inverse_key(P, P_inv)) {
        printf("Matrix inverse does not exist. Plaintext matrix is not invertible.\n");
        exit(1);
    }

    // Multiply C * P?¹ to get key matrix
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            recovered_key[i][j] = 0;
            for (int k = 0; k < 2; k++) {
                recovered_key[i][j] += C[i][k] * P_inv[k][j];
            }
            recovered_key[i][j] %= MOD;
        }
    }
}

int main() {
    int key[2][2] = {{3, 3}, {2, 5}}; // Example 2x2 key
    int inv_key[2][2];
    char plaintext[] = "HELP";
    char ciphertext[5] = {0};
    char decrypted[5] = {0};

    // Encrypt plaintext
    encrypt_block(&plaintext[0], key, &ciphertext[0]);
    encrypt_block(&plaintext[2], key, &ciphertext[2]);
    printf("Ciphertext: %s\n", ciphertext);

    // Decrypt ciphertext
    if (!inverse_key(key, inv_key)) {
        printf("Key is not invertible!\n");
        return 1;
    }
    decrypt_block(&ciphertext[0], inv_key, &decrypted[0]);
    decrypt_block(&ciphertext[2], inv_key, &decrypted[2]);
    printf("Decrypted: %s\n", decrypted);

    // Simulate known plaintext attack
    int recovered_key[2][2];
    known_plaintext_attack(plaintext, ciphertext, recovered_key);
    printf("Recovered key matrix:\n");
    printf("%d %d\n%d %d\n", recovered_key[0][0], recovered_key[0][1], recovered_key[1][0], recovered_key[1][1]);

    return 0;
}

