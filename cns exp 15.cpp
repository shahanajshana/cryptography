#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define ALPHABET_SIZE 26
#define MAX_CIPHERTEXT_LENGTH 1000



// Function to convert a letter to an index (A=0, B=1, ..., Z=25)
int letter_to_index(char c) {
    return tolower(c) - 'a';
}

// Function to decrypt the ciphertext with a given shift
void decrypt_with_shift(char* ciphertext, int shift, char* plaintext) {
    int i = 0;
    while (ciphertext[i] != '\0') {
        if (isalpha(ciphertext[i])) {
            char base = isupper(ciphertext[i]) ? 'A' : 'a';
            plaintext[i] = (ciphertext[i] - base - shift + ALPHABET_SIZE) % ALPHABET_SIZE + base;
        } else {
            plaintext[i] = ciphertext[i];  // Copy non-alphabet characters as is
        }
        i++;
    }
    plaintext[i] = '\0';
}

// Function to calculate the score of a plaintext based on letter frequencies
double calculate_score(char* plaintext) {
    int letter_count[ALPHABET_SIZE] = {0};
    int total_letters = 0;

    // Count the frequency of each letter in the plaintext
    for (int i = 0; plaintext[i] != '\0'; i++) {
        if (isalpha(plaintext[i])) {
            letter_count[letter_to_index(plaintext[i])]++;
            total_letters++;
        }
    }

    // Calculate the score by comparing the observed frequencies with expected frequencies
    double score = 0.0;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        double observed_freq = (double)letter_count[i] / total_letters;
        score += (observed_freq - english_frequencies[i]) * (observed_freq - english_frequencies[i]);
    }

    return score;
}

// Function to perform a letter frequency attack on an additive cipher
void frequency_attack(char* ciphertext, int top_n) {
    char plaintext[MAX_CIPHERTEXT_LENGTH];
    double scores[ALPHABET_SIZE];
    char decrypted_texts[ALPHABET_SIZE][MAX_CIPHERTEXT_LENGTH];
    
    // Decrypt the ciphertext with all possible shifts (0 to 25)
    for (int shift = 0; shift < ALPHABET_SIZE; shift++) {
        decrypt_with_shift(ciphertext, shift, plaintext);
        scores[shift] = calculate_score(plaintext);
        strcpy(decrypted_texts[shift], plaintext);
    }

    // Sort the shifts by score (lower score is better)
    int shift_order[ALPHABET_SIZE];
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        shift_order[i] = i;
    }

    // Sorting using bubble sort (for simplicity)
    for (int i = 0; i < ALPHABET_SIZE - 1; i++) {
        for (int j = i + 1; j < ALPHABET_SIZE; j++) {
            if (scores[shift_order[i]] > scores[shift_order[j]]) {
                int temp = shift_order[i];
                shift_order[i] = shift_order[j];
                shift_order[j] = temp;
            }
        }
    }

    // Output the top N most likely plaintexts
    printf("Top %d possible plaintexts:\n", top_n);
    for (int i = 0; i < top_n; i++) {
        printf("Shift %d: %s\n", shift_order[i], decrypted_texts[shift_order[i]]);
    }
}

int main() {
    char ciphertext[MAX_CIPHERTEXT_LENGTH];
    int top_n;

    // Get the ciphertext from the user
    printf("Enter the ciphertext: ");
    fgets(ciphertext, MAX_CIPHERTEXT_LENGTH, stdin);
    ciphertext[strcspn(ciphertext, "\n")] = '\0';  // Remove newline character

    // Get the number of top plaintexts to display
    printf("Enter the number of top plaintexts to show: ");
    scanf("%d", &top_n);

    // Perform the letter frequency attack and display the top N possible plaintexts
    frequency_attack(ciphertext, top_n);

    return 0;
}

