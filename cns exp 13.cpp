#include <stdio.h>
#include <stdlib.h>
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

// Function to perform matrix multiplication
void matrix_multiply(int A[2][2], int B[2][2], int result[2][2]) {
    result[0][0] = (A[0][0] * B[0][0] + A[0][1] * B[1][0]) % MOD;
    result[0][1] = (A[0][0] * B[0][1] + A[0][1] * B[1][1]) % MOD;
    result[1][0] = (A[1][0] * B[0][0] + A[1][1] * B[1][0]) % MOD;
    result[1][1] = (A[1][0] * B[0][1] + A[1][1] * B[1][1]) % MOD;
}

// Function to solve for the key matrix K from known plaintext-ciphertext pairs
void attack_hill_cipher(char plaintext[], char ciphertext[], int key[2][2]) {
    int P[2][2], C[2][2];  // 2x2 matrices for plaintext and ciphertext
    int K[2][2];  // The key matrix we're trying to solve for
    
    // Convert plaintext and ciphertext into numerical values
    P[0][0] = letter_to_number(plaintext[0]);
    P[0][1] = letter_to_number(plaintext[1]);
    P[1][0] = letter_to_number(plaintext[2]);
    P[1][1] = letter_to_number(plaintext[3]);

    C[0][0] = letter_to_number(ciphertext[0]);
    C[0][1] = letter_to_number(ciphertext[1]);
    C[1][0] = letter_to_number(ciphertext[2]);
    C[1][1] = letter_to_number(ciphertext[3]);

    // Now we have the system K * P = C. Solve for K by multiplying C by P's inverse
    int P_inv[2][2];
    inverse_matrix(P, P_inv);

    matrix_multiply(P_inv, C, K);

    // Display the key matrix
    printf("Key Matrix (K):\n");
    printf("%d %d\n", K[0][0], K[0][1]);
    printf("%d %d\n", K[1][0], K[1][1]);
}

int main() {
    char plaintext[] = "th";
    char ciphertext[] = "pm";  // Example of a known plaintext-ciphertext pair

    int key[2][2];  // To store the key matrix

    // Perform the attack to recover the key matrix
    attack_hill_cipher(plaintext, ciphertext, key);

    return 0;
}

