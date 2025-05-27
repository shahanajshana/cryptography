#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TEXT 1000
#define ALPHABET_SIZE 26

// Standard English letter frequency (most common to least)
const char english_freq_order[ALPHABET_SIZE + 1] = "ETAOINSHRDLCUMWFGYPBVKJXQZ";

// Count frequencies of letters in ciphertext
void count_frequencies(const char *text, int *freq) {
    for (int i = 0; text[i]; i++) {
        if (isupper(text[i])) {
            freq[text[i] - 'A']++;
        }
    }
}

// Sort indices of frequency array in decreasing order
void sort_freq_indices(int *freq, int *indices) {
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        indices[i] = i;
    }

    // Sort indices based on freq using bubble sort (small size, simple)
    for (int i = 0; i < ALPHABET_SIZE - 1; i++) {
        for (int j = 0; j < ALPHABET_SIZE - i - 1; j++) {
            if (freq[indices[j]] < freq[indices[j + 1]]) {
                int temp = indices[j];
                indices[j] = indices[j + 1];
                indices[j + 1] = temp;
            }
        }
    }
}

// Generate substitution map from frequency analysis
void generate_map(int *sorted_indices, char *sub_map, const char *guess_freq) {
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        sub_map[sorted_indices[i]] = guess_freq[i];
    }
}

// Apply substitution map to ciphertext
void decrypt_with_map(const char *ciphertext, char *plaintext, char *sub_map) {
    for (int i = 0; ciphertext[i]; i++) {
        if (isupper(ciphertext[i])) {
            plaintext[i] = sub_map[ciphertext[i] - 'A'];
        } else {
            plaintext[i] = ciphertext[i];
        }
    }
    plaintext[strlen(ciphertext)] = '\0';
}

int main() {
    char ciphertext[MAX_TEXT];
    char plaintext[MAX_TEXT];
    int freq[ALPHABET_SIZE] = {0};
    int sorted_indices[ALPHABET_SIZE];
    char sub_map[ALPHABET_SIZE];
    int top_n;

    printf("Enter ciphertext in UPPERCASE (no spaces/punctuation if possible):\n");
    fgets(ciphertext, MAX_TEXT, stdin);
    ciphertext[strcspn(ciphertext, "\n")] = '\0';

    printf("Enter number of top plaintext guesses to display: ");
    scanf("%d", &top_n);

    // Step 1: Count letter frequencies
    count_frequencies(ciphertext, freq);

    // Step 2: Sort frequencies
    sort_freq_indices(freq, sorted_indices);

    // Step 3: Generate and test multiple frequency maps
    for (int i = 0; i < top_n && i < ALPHABET_SIZE; i++) {
        char guess_freq[ALPHABET_SIZE + 1];
        // Slightly rotate the guess to generate different candidates
        for (int j = 0; j < ALPHABET_SIZE; j++) {
            guess_freq[j] = english_freq_order[(j + i) % ALPHABET_SIZE];
        }
        guess_freq[ALPHABET_SIZE] = '\0';

        generate_map(sorted_indices, sub_map, guess_freq);
        decrypt_with_map(ciphertext, plaintext, sub_map);

        printf("\n--- Candidate #%d ---\n", i + 1);
        printf("Guessed mapping:\n");
        for (int k = 0; k < ALPHABET_SIZE; k++) {
            printf("%c -> %c\n", 'A' + k, sub_map[k]);
        }
        printf("Decrypted Plaintext:\n%s\n", plaintext);
    }

    return 0;
}

