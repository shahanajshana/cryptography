#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define ALPHABET_LEN 26
void generateCipherAlphabet(const char* keyword, char cipherMap[26]) {
    int used[26] = {0}; 
    int i, j = 0;
    for (i = 0; keyword[i] != '\0'; i++) {
        char c = toupper(keyword[i]);
        if (isalpha(c) && !used[c - 'A']) {
            cipherMap[j++] = c;
            used[c - 'A'] = 1;
        }
    }
    for (i = 0; i < 26; i++) {
        if (!used[i]) {
            cipherMap[j++] = 'A' + i;
        }
    }
}
void encryptText(const char* plaintext, const char cipherMap[26], char* ciphertext) {
    for (int i = 0; plaintext[i]; i++) {
        char c = plaintext[i];
        if (isalpha(c)) {
            int index = tolower(c) - 'a';
            ciphertext[i] = isupper(c) ? cipherMap[index] : tolower(cipherMap[index]);
        } else {
            ciphertext[i] = c;  
        }
    }
    ciphertext[strlen(plaintext)] = '\0';
}
void decryptText(const char* ciphertext, const char cipherMap[26], char* plaintext) {
    for (int i = 0; ciphertext[i]; i++) {
        char c = ciphertext[i];
        if (isalpha(c)) {
            char uc = toupper(c);
            for (int j = 0; j < ALPHABET_LEN; j++) {
                if (cipherMap[j] == uc) {
                    plaintext[i] = isupper(c) ? 'A' + j : 'a' + j;
                    break;
                }
            }
        } else {
            plaintext[i] = c;
        }
    }
    plaintext[strlen(ciphertext)] = '\0';
}

int main() {
    char keyword[100];
    char cipherMap[26];
    char plaintext[500], ciphertext[500], decrypted[500];

    printf("Enter the keyword: ");
    scanf("%s", keyword);
    getchar();  // Clear newline

    printf("Enter the plaintext: ");
    fgets(plaintext, sizeof(plaintext), stdin);
    plaintext[strcspn(plaintext, "\n")] = '\0';

    generateCipherAlphabet(keyword, cipherMap);

    // Show cipher alphabet
    printf("\nCipher Alphabet:\n");
    printf("Plain : ");
    for (int i = 0; i < 26; i++) printf("%c ", 'A' + i);
    printf("\nCipher: ");
    for (int i = 0; i < 26; i++) printf("%c ", cipherMap[i]);
    printf("\n");

    encryptText(plaintext, cipherMap, ciphertext);
    decryptText(ciphertext, cipherMap, decrypted);

    printf("\nEncrypted Text: %s", ciphertext);
    printf("\nDecrypted Text: %s\n", decrypted);

    return 0;
}

