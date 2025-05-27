#include <stdio.h>

// Function to calculate factorial of a number
unsigned long long factorial(int n) {
    unsigned long long fact = 1;
    for (int i = 1; i <= n; i++) {
        fact *= i;
    }
    return fact;
}

int main() {
    // Total number of letters in Playfair cipher grid (usually 25 after treating I/J as the same)
    int n = 25;

    // Calculate the total number of possible keys (permutations of 25 distinct letters)
    unsigned long long total_keys = factorial(n);

    // Print the total possible keys
    printf("Total possible Playfair keys: %llu\n", total_keys);

    // Approximate as a power of 2 for simplicity
    unsigned long long power_of_2 = 0;
    unsigned long long temp = total_keys;
    while (temp > 1) {
        temp >>= 1; // Divide by 2
        power_of_2++;
    }

    // Print the approximate power of 2
    printf("Approximate power of 2: 2^%llu\n", power_of_2);

    return 0;
}

