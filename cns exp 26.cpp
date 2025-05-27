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

// Extended Euclidean Algorithm to find modular inverse
int extended_euclidean(int e, int phi, int *x, int *y) {
    if (phi == 0) {
        *x = 1;
        *y = 0;
        return e;
    }
    int x1, y1;
    int gcd = extended_euclidean(phi, e % phi, &x1, &y1);
    *x = y1;
    *y = x1 - (e / phi) * y1;
    return gcd;
}

// Compute the private key given e and phi
int modular_inverse(int e, int phi) {
    int x, y;
    int gcd = extended_euclidean(e, phi, &x, &y);
    if (gcd != 1) {
        return -1; // Modular inverse does not exist
    }
    return (x % phi + phi) % phi; // Ensure positive result
}

// Factorize n using known d
void factorize_from_private_key(int e, int d, int n) {
    // Compute phi(n) from e and d
    int k = (e * d - 1); // k must be divisible by phi(n)
    for (int t = 1; t <= n; t++) {
        if (k % t == 0) {
            int phi = k / t;

            // Solve for p and q using phi and n
            int discriminant = (n - phi + 1) * (n - phi + 1) - 4 * n;
            if (discriminant >= 0) {
                int sqrt_discriminant = (int)(sqrt(discriminant));
                if (sqrt_discriminant * sqrt_discriminant == discriminant) {
                    int p = (n - phi + 1 + sqrt_discriminant) / 2;
                    int q = (n - phi + 1 - sqrt_discriminant) / 2;

                    if (p * q == n) {
                        printf("Factors found: p = %d, q = %d\n", p, q);
                        return;
                    }
                }
            }
        }
    }
    printf("Failed to factorize n using leaked private key.\n");
}

int main() {
    int n = 3599;  // Example RSA modulus
    int e = 31;    // Public key
    int d = 247;   // Private key (leaked)

    printf("Modulus (n): %d\n", n);
    printf("Public key (e): %d\n", e);
    printf("Private key (d): %d (leaked)\n", d);

    // Factorize n using the leaked private key
    factorize_from_private_key(e, d, n);

    // Conclusion
    printf("Conclusion: Generating new keys with the same modulus is NOT safe.\n");

    return 0;
}

