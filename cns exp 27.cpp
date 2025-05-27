#include <stdio.h>
#include <math.h>

// Function to perform modular exponentiation
long long mod_exp(long long base, long long exp, long long mod) {
    long long result = 1;
    while (exp > 0) {
        if (exp % 2 == 1) {
            result = (result * base) % mod;
        }
        base = (base * base) % mod;
        exp /= 2;
    }
    return result;
}

int main() {
    // Example RSA parameters
    long long n = 3599;    // Modulus (very large in real cases)
    long long e = 31;      // Public key (exponent)

    // Ciphertext intercepted (example)
    long long intercepted_ciphertext = 215; 

    printf("Intercepted ciphertext: %lld\n", intercepted_ciphertext);

    // Precompute all possible plaintexts and their ciphertexts
    printf("Performing brute-force attack...\n");
    for (int m = 0; m < 26; m++) {
        long long c = mod_exp(m, e, n);
        if (c == intercepted_ciphertext) {
            printf("Decrypted plaintext: %d (character: %c)\n", m, 'A' + m);
            return 0;
        }
    }

    printf("Attack failed. Ciphertext does not match any plaintext in the range 0-25.\n");
    return 0;
}
#include <stdio.h>
#include <math.h>

// Function to compute modular exponentiation
long long mod_exp(long long base, long long exp, long long mod) {
    long long result = 1;
    while (exp > 0) {
        if (exp % 2 == 1) {
            result = (result * base) % mod;
        }
        base = (base * base) % mod;
        exp /= 2;
    }
    return result;
}

int main() {
    // Public numbers
    long long a = 5;    // Public base
    long long q = 23;   // Public prime modulus

    // Secret numbers chosen by Alice and Bob
    long long alice_secret = 6; // Alice's secret
    long long bob_secret = 15;  // Bob's secret

    // Step 1: Compute values to send
    long long alice_sends = mod_exp(a, alice_secret, q); // Alice sends a^x mod q
    long long bob_sends = mod_exp(a, bob_secret, q);     // Bob sends a^x mod q

    printf("Alice sends: %lld\n", alice_sends);
    printf("Bob sends: %lld\n", bob_sends);

    // Step 2: Attempt to compute the shared key
    long long alice_shared_key = mod_exp(bob_sends, alice_secret, q);
    long long bob_shared_key = mod_exp(alice_sends, bob_secret, q);

    printf("Alice computes shared key: %lld\n", alice_shared_key);
    printf("Bob computes shared key: %lld\n", bob_shared_key);

    // Vulnerability demonstration (Eve computes the secret key directly)
    long long eve_key = mod_exp(a, (alice_secret + bob_secret), q);
    printf("Eve computes shared key (vulnerable): %lld\n", eve_key);

    return 0;
}

