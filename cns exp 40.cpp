#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define ALPHABET_SIZE 26
#define MAX_TEXT 1000
#define MAX_CANDIDATES 100

// English letter frequencies (A-Z)
const double english_freq[ALPHABET_SIZE] = {
    8.167, 1.492, 2.782, 4.253, 12.702,
    2.228, 2.015, 6.094, 6.966, 0.153,
    0.772, 4.025, 2.406, 6.749, 7.507,
    1.929, 0.095, 5.987, 6.327, 9.056,
    2.758, 0.978, 2.360, 0.150, 1.974,
    0.074
};

typedef struct {
    char plaintext[MAX_TEXT];
    double score;
} Candidate;

// Calculate letter frequencies in text
void calc_freq(const char *text, double *freq) {
    int counts[ALPHABET_SIZE] = {0};
    int length = 0;
    for (int i = 0; text[i]; i++) {
        if (isupper(text[i])) {
            counts[text[i] - 'A']++;
            length++;
        }
    }
    if (length == 0) {
        for (int i = 0; i < ALPHABET_SIZE; i++) freq[i] = 0.0;
        return;
    }
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        freq[i] = 100.0 * counts[i] / length;
    }
}

// Compute sum of absolute freq difference (lower is better)
double score_text(const char *text) {
    double freq[ALPHABET_SIZE];
    calc_freq(text, freq);
    double score = 0.0;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        double d = freq[i] - english_freq[i];
        if (d < 0) d = -d;
        score += d;
    }
    return score;
}

// Sort candidates ascending by score
int compare_candidates(const void *a, const void *b) {
    Candidate *ca = (Candidate *)a;
    Candidate *cb = (Candidate *)b;
    if (ca->score < cb->score) return -1;
    else if (ca->score > cb->score) return 1;
    else return 0;
}

// Map ciphertext letters to English letters by frequency order
void freq_map(const char *ciphertext, char *mapping) {
    int counts[ALPHABET_SIZE] = {0};
    int len = strlen(ciphertext);

    // Count frequency of ciphertext letters
    for (int i = 0; i < len; i++) {
        if (isupper(ciphertext[i]))
            counts[ciphertext[i] - 'A']++;
    }

    // Create array of ciphertext letters sorted by freq descending
    int indices[ALPHABET_SIZE];
    for (int i = 0; i < ALPHABET_SIZE; i++) indices[i] = i;

    // Simple bubble sort by counts descending
    for (int i = 0; i < ALPHABET_SIZE - 1; i++) {
        for (int j = 0; j < ALPHABET_SIZE - 1 - i; j++) {
            if (counts[indices[j]] < counts[indices[j + 1]]) {
                int tmp = indices[j];
                indices[j] = indices[j + 1];
                indices[j + 1] = tmp;
            }
        }
    }

    // Map ciphertext letters to English letters by freq order
    const char *eng_order = "ETAOINSHRDLCUMWFGYPBVKJXQZ";
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        mapping[indices[i]] = eng_order[i];
    }
}

// Apply mapping to ciphertext to get plaintext guess
void apply_mapping(const char *ciphertext, char *plaintext, const char *mapping) {
    int len = strlen(ciphertext);
    for (int i = 0; i < len; i++) {
        if (isupper(ciphertext[i])) {
            plaintext[i] = mapping[ciphertext[i] - 'A'];
        } else {
            plaintext[i] = ciphertext[i];
        }
    }
    plaintext[len] = '\0';
}

// Swap two chars in mapping
void swap_chars(char *mapping, char a, char b) {
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (mapping[i] == a) mapping[i] = b;
        else if (mapping[i] == b) mapping[i] = a;
    }
}

int main() {
    char ciphertext[MAX_TEXT];
    printf("Enter ciphertext (uppercase letters only):\n");
    fgets(ciphertext, MAX_TEXT, stdin);
    ciphertext[strcspn(ciphertext, "\n")] = 0;

    int top_n;
    printf("Enter number of top plaintext guesses to display (up to 10): ");
    scanf("%d", &top_n);
    if (top_n < 1) top_n = 1;
    if (top_n > 10) top_n = 10;

    char base_mapping[ALPHABET_SIZE];
    freq_map(ciphertext, base_mapping);

    Candidate candidates[MAX_CANDIDATES];
    int candidate_count = 0;

    // Generate candidates by swapping pairs of letters in top freq letters
    // to generate some variations (brute force limited to first 6 letters to keep runtime reasonable)
    const int max_swaps = 6;
    int swap_letters = 6;

    // Start with base mapping
    apply_mapping(ciphertext, candidates[candidate_count].plaintext, base_mapping);
    candidates[candidate_count].score = score_text(candidates[candidate_count].plaintext);
    candidate_count++;

    // Generate candidates by swapping pairs of letters among the top swap_letters in base_mapping
    for (int i = 0; i < swap_letters; i++) {
        for (int j = i + 1; j < swap_letters && candidate_count < MAX_CANDIDATES; j++) {
            char new_mapping[ALPHABET_SIZE];
            memcpy(new_mapping, base_mapping, ALPHABET_SIZE);

            // Swap letters at positions i and j
            char li = base_mapping[i];
            char lj = base_mapping[j];
            swap_chars(new_mapping, li, lj);

            apply_mapping(ciphertext, candidates[candidate_count].plaintext, new_mapping);
            candidates[candidate_count].score = score_text(candidates[candidate_count].plaintext);
            candidate_count++;
        }
    }

    // Sort candidates by score ascending (best first)
    qsort(candidates, candidate_count, sizeof(Candidate), compare_candidates);

    // Print top N candidates
    printf("\nTop %d candidate plaintexts:\n", top_n);
    for (int i = 0; i < top_n && i < candidate_count; i++) {
        printf("Score: %.2f\n%s\n\n", candidates[i].score, candidates[i].plaintext);
    }

    return 0;
}

