#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define SIZE 5

char matrix[SIZE][SIZE] = {
    {'M', 'F', 'H', 'I', 'K'},
    {'U', 'N', 'O', 'P', 'Q'},
    {'Z', 'V', 'W', 'X', 'Y'},
    {'E', 'L', 'A', 'R', 'G'},
    {'D', 'S', 'T', 'B', 'C'}
};
void findPosition(char ch, int *row, int *col) {
    if (ch == 'J') ch = 'I';  
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            if (matrix[i][j] == ch) {
                *row = i;
                *col = j;
                return;
            }
}
void prepareText(const char* input, char* output) {
    int len = 0;
    for (int i = 0; input[i]; i++) {
        if (isalpha(input[i])) {
            output[len++] = toupper(input[i] == 'J' ? 'I' : input[i]);
        }
    }
    output[len] = '\0';
    char temp[500];
    int j = 0;
    for (int i = 0; i < len; i++) {
        temp[j++] = output[i];
        if (i < len - 1 && output[i] == output[i + 1]) {
            temp[j++] = 'X';  // filler
        }
    }
    if (j % 2 != 0) {
        temp[j++] = 'X';  // padding
    }
    temp[j] = '\0';
    strcpy(output, temp);
}

// Encrypt digraph pair
void encryptPair(char a, char b, char* out1, char* out2) {
    int r1, c1, r2, c2;
    findPosition(a, &r1, &c1);
    findPosition(b, &r2, &c2);

    if (r1 == r2) {
        *out1 = matrix[r1][(c1 + 1) % SIZE];
        *out2 = matrix[r2][(c2 + 1) % SIZE];
    } else if (c1 == c2) {
        *out1 = matrix[(r1 + 1) % SIZE][c1];
        *out2 = matrix[(r2 + 1) % SIZE][c2];
    } else {
        *out1 = matrix[r1][c2];
        *out2 = matrix[r2][c1];
    }
}

// Encrypt full message
void encryptText(const char* plaintext, char* ciphertext) {
    char prepared[500];
    prepareText(plaintext, prepared);

    int len = strlen(prepared), j = 0;
    for (int i = 0; i < len; i += 2) {
        char a = prepared[i];
        char b = prepared[i + 1];
        encryptPair(a, b, &ciphertext[j], &ciphertext[j + 1]);
        j += 2;
    }
    ciphertext[j] = '\0';
}

int main() {
    const char* plaintext = "Must see you over Cadogan West. Coming at once.";
    char ciphertext[500];

    printf("Plaintext: %s\n", plaintext);

    encryptText(plaintext, ciphertext);

    printf("Encrypted (Playfair): %s\n", ciphertext);

    return 0;
}
