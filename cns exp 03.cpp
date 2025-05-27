#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define SIZE 5

char matrix[SIZE][SIZE];

void prepareKeyMatrix(char key[]) {
    int dict[26] = {0};
    int i, j, k = 0;
    
    dict['J' - 'A'] = 1; 
    for (i = 0; i < strlen(key); i++) {
        if (key[i] == 'J') key[i] = 'I';
        if (!dict[key[i] - 'A']) {
            matrix[k / SIZE][k % SIZE] = key[i];
            dict[key[i] - 'A'] = 1;
            k++;
        }
    }

    for (i = 0; i < 26; i++) {
        if (!dict[i]) {
            matrix[k / SIZE][k % SIZE] = i + 'A';
            k++;
        }
    }
}
void formatPlaintext(char* text, char* formatted) {
    int i, j = 0;
    for (i = 0; i < strlen(text); i++) {
        if (!isalpha(text[i])) continue;
        text[i] = toupper(text[i]);
        if (text[i] == 'J') text[i] = 'I';

        if (j > 0 && formatted[j - 1] == text[i]) {
            formatted[j++] = 'X';
        }
        formatted[j++] = text[i];
    }
    if (j % 2 != 0) {
        formatted[j++] = 'X';  // padding
    }
    formatted[j] = '\0';
}

void findPosition(char c, int* row, int* col) {
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            if (matrix[i][j] == c) {
                *row = i;
                *col = j;
                return;
            }
}

void encrypt(char* plaintext, char* ciphertext) {
    int i, r1, c1, r2, c2;
    for (i = 0; i < strlen(plaintext); i += 2) {
        findPosition(plaintext[i], &r1, &c1);
        findPosition(plaintext[i + 1], &r2, &c2);

        if (r1 == r2) {
            ciphertext[i]     = matrix[r1][(c1 + 1) % SIZE];
            ciphertext[i + 1] = matrix[r2][(c2 + 1) % SIZE];
        } else if (c1 == c2) {
            ciphertext[i]     = matrix[(r1 + 1) % SIZE][c1];
            ciphertext[i + 1] = matrix[(r2 + 1) % SIZE][c2];
        } else {
            ciphertext[i]     = matrix[r1][c2];
            ciphertext[i + 1] = matrix[r2][c1];
        }
    }
    ciphertext[i] = '\0';
}

void printMatrix() {
    printf("\nPlayfair Matrix:\n");
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            printf("%c ", matrix[i][j]);
        }
        printf("\n");
    }
}

int main() {
    char key[100], text[100], formatted[200], ciphertext[200];

    printf("Enter the key: ");
    scanf("%s", key);

    printf("Enter plaintext: ");
    scanf("%s", text);

    
    for (int i = 0; key[i]; i++) key[i] = toupper(key[i]);
    
    prepareKeyMatrix(key);
    printMatrix();

    formatPlaintext(text, formatted);
    encrypt(formatted, ciphertext);

    printf("\nFormatted Plaintext: %s", formatted);
    printf("\nEncrypted Ciphertext: %s\n", ciphertext);

    return 0;
}

