#include <stdio.h>

// Function to compute gcd using Euclid's algorithm
int gcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

int main() {
    int n = 3599;      // Given RSA modulus
    int e = 31;        // Given public key
    int plaintext_block = 1180; // Example plaintext block with a common factor

    printf("RSA modulus (n): %d\n", n);
    printf("Public key (e): %d\n", e);
    printf("Plaintext block: %d\n", plaintext_block);

    // Step 1: Compute gcd of the plaintext block and n
    int factor = gcd(plaintext_block, n);

    if (factor > 1 && factor < n) {
        // Factorization successful
        int p = factor;
        int q = n / factor;
        printf("Factorization successful!\n");
        printf("p = %d, q = %d\n", p, q);

        // Step 2: Compute phi(n)
        int phi = (p - 1) * (q - 1);
        printf("phi(n) = %d\n", phi);

        // Step 3: Compute the private key d
        // Using modular inverse (as implemented in previous examples)
        int x, y;
        int gcd_res = 0;
        for (int d = 1; d < phi; d++) {
            // Verify modular inverse
            if (((e * d) % phi) == 1) {
                printf("Private key (d) = %d\n", d);
                break;
            }
        }
    } else {
        printf("No common factor found. RSA remains secure.\n");
    }

    return 0;
}

