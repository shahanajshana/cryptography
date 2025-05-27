#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define SIZE 5

char matrix[SIZE][SIZE];
void prepareKeyMatrix(char *key) {
    int dict[26] = {0};
    int k = 0;

    dict['J' - 'A'] = 1; 

    for (int i = 0; i < strlen(key); i++) {
        char c = toupper(key[i]);
        if (c == 'J') c = 'I';
        if (!dict[c - 'A']) {
            matrix[k / SIZE][k % SIZE] = c;
            dict[c - 'A'] = 1;
            k++;
        }
    }

    for (int i = 0; i < 26; i++) {
        if (!dict[i]) {
            matrix[k / SIZE][k % SIZE] = 'A' + i;
            dict[i] = 1;
            k++;
        }
    }
}

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

void decryptPair(char a, char b, char *out1, char *out2) {
    int r1, c1, r2, c2;
    findPosition(a, &r1, &c1);
    findPosition(b, &r2, &c2);

    if (r1 == r2) {
        *out1 = matrix[r1][(c1 + SIZE - 1) % SIZE];
        *out2 = matrix[r2][(c2 + SIZE - 1) % SIZE];
    } else if (c1 == c2) {
        *out1 = matrix[(r1 + SIZE - 1) % SIZE][c1];
        *out2 = matrix[(r2 + SIZE - 1) % SIZE][c2];
    } else {
        *out1 = matrix[r1][c2];
        *out2 = matrix[r2][c1];
    }
}

void decryptPlayfair(char *cipher, char *output) {
    int i, j = 0;
    for (i = 0; i < strlen(cipher); i += 2) {
        char a = toupper(cipher[i]);
        char b = toupper(cipher[i + 1]);
        if (!isalpha(a) || !isalpha(b)) continue;
        char dec1, dec2;
        decryptPair(a, b, &dec1, &dec2);
        output[j++] = dec1;
        output[j++] = dec2;
    }
    output[j] = '\0';
}

void printMatrix() {
    printf("\nPlayfair Matrix:\n");
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++)
            printf("%c ", matrix[i][j]);
        printf("\n");
    }
}

int main() {
    char key[] = "PTBOAT"; // likely key used for PT-109 related message
    char cipher[] =
        "KXJEYUREBEZWEHEWRYTUHEYFSKREHEGOYFIWTTTUOLKSYCAJPO"
        "BOTEIZONTXBYBNTGONEYCUZWRGDSONSXBOUYWRHEBAAHYUSEDQ";
    char decrypted[500];

    prepareKeyMatrix(key);
    printMatrix();

    decryptPlayfair(cipher, decrypted);

    printf("\nDecrypted Message:\n%s\n", decrypted);
    return 0;
}

