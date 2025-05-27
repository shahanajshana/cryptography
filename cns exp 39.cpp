#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define ALPHABET_SIZE 26
#define MAX_TEXT 1000

// English letter frequencies (approximate percentages)
// Order: A-Z
const double english_freq[ALPHABET_SIZE] = {
    8.167, 1.492, 2.782, 4.253, 12.702,
    2.228, 2.015, 6.094, 6.966, 0.153,
    0.772, 4.025, 2.406, 6.749, 7.507,
    1.929, 0.095, 5.987, 6.327, 9.056,
    2.758, 0.978, 2.360, 0.150, 1.974,
    0.074
};

// Calculate letter frequencies of given text (only uppercase letters considered)
void calculate_freq(const char *text, double *freq, int *total_letters) {
    int counts[ALPHABET_SIZE] = {0};
    int length = 0;
    for (int i = 0; text[i]; i++) {
        if (isupper(text[i])) {
            counts[text[i] - 'A']++;
            length++;
        }
    }
    *total_letters = length;
    if (length == 0) {
        for (int i = 0; i < ALPHABET_SIZE; i++) freq[i] = 0.0;
        return;
    }
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        freq[i] = ((double)counts[i]) * 100.0 / length;
    }
}

// Score candidate by summing absolute differences of letter frequencies vs English
double score_text(const char *text) {
    double freq[ALPHABET_SIZE];
    int total_letters;
    calculate_freq(text, freq, &total_letters);

    double score = 0.0;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        double diff = freq[i] - english_freq[i];
        if (diff < 0) diff = -diff;
        score += diff;
    }
    return score; // Lower score means closer match to English freq
}

// Decrypt ciphertext with given shift key
void decrypt_caesar(const char *ciphertext, char *plaintext, int key) {
    for (int i = 0; ciphertext[i]; i++) {
        if (isupper(ciphertext[i])) {
            plaintext[i] = ((ciphertext[i] - 'A' - key + ALPHABET_SIZE) % ALPHABET_SIZE) + 'A';
        } else {
            plaintext[i] = ciphertext[i];
        }
    }
    plaintext[strlen(ciphertext)] = '\0';
}

typedef struct {
    double score;
    int key;
    char plaintext[MAX_TEXT];
} Candidate;

void swap_candidates(Candidate *a, Candidate *b) {
    Candidate temp = *a;
    *a = *b;
    *b = temp;
}

void sort_candidates(Candidate *candidates, int n) {
    // Simple bubble sort by score ascending (better scores first)
    for (int i = 0; i < n-1; i++) {
        for (int j = 0; j < n-1-i; j++) {
            if (candidates[j].score > candidates[j+1].score) {
                swap_candidates(&candidates[j], &candidates[j+1]);
            }
        }
    }
}

int main() {
    char ciphertext[MAX_TEXT];
    printf("Enter ciphertext (uppercase letters only):\n");
    fgets(ciphertext, MAX_TEXT, stdin);
    ciphertext[strcspn(ciphertext, "\n")] = '\0';

    int top_n;
    printf("Enter number of top plaintext guesses to display: ");
    scanf("%d", &top_n);
    if (top_n < 1 || top_n > ALPHABET_SIZE) {
        printf("Number must be between 1 and 26.\n");
        return 1;
    }

    Candidate candidates[ALPHABET_SIZE];

    for (int key = 0; key < ALPHABET_SIZE; key++) {
        decrypt_caesar(ciphertext, candidates[key].plaintext, key);
        candidates[key].score = score_text(candidates[key].plaintext);
        candidates[key].key = key;
    }

    sort_candidates(candidates, ALPHABET_SIZE);

    printf("\nTop %d guesses:\n", top_n);
    for (int i = 0; i < top_n; i++) {
        printf("Key = %2d | Score = %.2f | Plaintext: %s\n",
            candidates[i].key, candidates[i].score, candidates[i].plaintext);
    }

    return 0;
}

