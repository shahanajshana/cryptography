#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define ALPHABET_SIZE 26
#define MAX_CIPHERTEXT_LENGTH 1000

// English letter frequencies (approximate frequencies)
const double english_frequencies[ALPHABET_SIZE] = {
    0.082, 0.015, 0.028, 0.043, 0.127, 0.022, 0.020, 0.061, 0.070, 0.002, 
    0.008, 0.040, 0.024, 0.067, 0.075, 0.019, 0.001, 0.060, 0.063, 0.091, 
    0.028, 0.010, 0.023, 0.001, 0.020, 0.002, 0.002, 0.004
};

// Function to convert a letter to an index (A=0, B=1, ..., Z=25)
int letter_to_index(char c) {
    return tolower(c) - 'a';
}

// Function to convert an index to a letter (0=A, 1=B, ..., 25=Z)
char index_to_letter(int index) {
    return (char)(index + 'a');
}

// Function to calculate the frequency of each letter in the ciphertext
void calculate_frequency(char* ciphertext, int* freq) {
    for (int i = 0; ciphertext[i] != '\0'; i++) {
        if (isalpha(ciphertext[i])) {
            int index = letter_to_index(ciphertext[i]);
            freq[index]++;
        }
    }
}

// Function to calculate the score based on frequency comparison with English frequencies
double calculate_score(int* freq) {
    int total_letters = 0;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        total_letters += freq[i];
    }

    double score = 0.0;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        double observed_freq = (double)freq[i] / total_letters;
        score += (observed_freq - english_frequencies[i]) * (observed_freq - english_frequencies[i]);
    }

    return score;
}

// Function to perform a letter frequency attack on a monoalphabetic substitution cipher
void frequency_attack(char* ciphertext, int top_n) {
    int freq[ALPHABET_SIZE] = {0};
    calculate_frequency(ciphertext, freq);
    
    // Create a list of letter indices sorted by frequency
    int sorted_indices[ALPHABET_SIZE];
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        sorted_indices[i] = i;
    }

    // Sort the letters based on frequency (bubble sort)
    for (int i = 0; i < ALPHABET_SIZE - 1; i++) {
        for (int j = i + 1; j < ALPHABET_SIZE; j++) {
            if (freq[sorted_indices[i]] < freq[sorted_indices[j]]) {
                int temp = sorted_indices[i];
                sorted_indices[i] = sorted_indices[j];
                sorted_indices[j] = temp;
            }
        }
    }

    // Create a mapping of cipher letter -> plain letter using the sorted frequencies
    char substitution[ALPHABET_SIZE];
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        substitution[sorted_indices[i]] = index_to_letter(i);
    }

    // Now we generate the possible plaintext
    char plaintext[MAX_CIPHERTEXT_LENGTH];
    for (int i = 0; ciphertext[i] != '\0'; i++) {
        if (isalpha(ciphertext[i])) {
            int index = letter_to_index(ciphertext[i]);
            plaintext[i] = substitution[index];
        } else {
            plaintext[i] = ciphertext[i];
        }
    }
    plaintext[strlen(ciphertext)] = '\0';

    // Print the result
    printf("Decrypted Text:\n%s\n", plaintext);
}

int main() {
    char ciphertext[MAX_CIPHERTEXT_LENGTH];
    int top_n;

    // Get the ciphertext from the user
    printf("Enter the ciphertext: ");
    fgets(ciphertext, MAX_CIPHERTEXT_LENGTH, stdin);
    ciphertext[strcspn(ciphertext, "\n")] = '\0';  // Remove newline character

    // Get the number of top plaintexts to display (for now just display 1 possible decryption)
    printf("Enter the number of top plaintexts to show: ");
    scanf("%d", &top_n);

    // Perform the letter frequency attack
    frequency_attack(ciphertext, top_n);

    return 0;
}

