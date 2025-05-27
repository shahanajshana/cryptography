#include <stdio.h>

// Function to find gcd using Euclid's algorithm
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

// Function to find the modular inverse of e modulo phi
int modular_inverse(int e, int phi) {
    int x, y;
    int gcd = extended_euclidean(e, phi, &x, &y);
    if (gcd != 1) {
        printf("Modular inverse does not exist.\n");
        return -1;
    }
    return (x % phi + phi) % phi; // Ensure result is positive
}

// Function to factorize n and find p and q
void factorize(int n, int *p, int *q) {
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) {
            *p = i;
            *q = n / i;
            return;
        }
    }
    *p = *q = -1; // If no factors found
}

int main() {
    int n = 3599;     // Given modulus
    int e = 31;       // Given public key
    int p, q;         // Prime factors of n
    int phi, d;       // Euler's totient function and private key

    // Step 1: Factorize n to find p and q
    factorize(n, &p, &q);

    if (p == -1 || q == -1) {
        printf("Failed to factorize n.\n");
        return 1;
    }

    printf("Prime factors of n: p = %d, q = %d\n", p, q);

    // Step 2: Compute phi(n)
    phi = (p - 1) * (q - 1);
    printf("phi(n) = %d\n", phi);

    // Step 3: Find the modular inverse of e modulo phi(n)
    d = modular_inverse(e, phi);

    if (d == -1) {
        printf("Failed to find the modular inverse.\n");
        return 1;
    }

    printf("Private key (d) = %d\n", d);

    return 0;
}


