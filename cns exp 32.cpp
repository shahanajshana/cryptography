#include <stdio.h>
#include <openssl/dsa.h>
#include <openssl/engine.h>
#include <openssl/sha.h>

void print_signature(const unsigned char *sig, unsigned int sig_len) {
    for (unsigned int i = 0; i < sig_len; i++) {
        printf("%02X", sig[i]);
    }
    printf("\n");
}

int main() {
    // Generate DSA key
    DSA *dsa = DSA_new();
    DSA_generate_parameters_ex(dsa, 1024, NULL, 0, NULL, NULL, NULL);
    DSA_generate_key(dsa);

    // Message to sign
    const char *message = "This is a test message.";
    unsigned char hash[SHA_DIGEST_LENGTH];
    SHA1((unsigned char *)message, strlen(message), hash);

    // First signature
    unsigned char sig1[DSA_size(dsa)];
    unsigned int sig1_len;
    DSA_sign(0, hash, SHA_DIGEST_LENGTH, sig1, &sig1_len, dsa);

    // Second signature (same message, new k internally)
    unsigned char sig2[DSA_size(dsa)];
    unsigned int sig2_len;
    DSA_sign(0, hash, SHA_DIGEST_LENGTH, sig2, &sig2_len, dsa);

    printf("First signature:  ");
    print_signature(sig1, sig1_len);

    printf("Second signature: ");
    print_signature(sig2, sig2_len);

    // Clean up
    DSA_free(dsa);
    return 0;
}

