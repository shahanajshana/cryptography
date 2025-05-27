#include <stdio.h>
#include <string.h>

#define MAX_TEXT 1000
char mapping[256]; 
int freq[256] = {0};
void countFrequency(const char *text) {
    for (int i = 0; text[i]; i++) {
        unsigned char c = text[i];
        freq[c]++;
    }

    printf("Character Frequencies:\n");
    for (int i = 0; i < 256; i++) {
        if (freq[i] > 0 && (i >= 32 && i <= 126)) {
            printf("'%c': %d\n", i, freq[i]);
        }
    }
}
void applyMapping(const char *ciphertext, char *output) {
    for (int i = 0; ciphertext[i]; i++) {
        char c = ciphertext[i];
        if (mapping[(unsigned char)c]) {
            output[i] = mapping[(unsigned char)c];
        } else {
            output[i] = c; 
        }
    }
    output[strlen(ciphertext)] = '\0';
}
void initializeMapping() {
    mapping['‡'] = 'e';  
    mapping[';'] = 't';  
    mapping['4'] = 'h';  
    mapping['8'] = 'o';  
    mapping['5'] = 's';
    mapping['*'] = 'n';
    mapping[')'] = 'r';
    mapping['3'] = 'd';
    mapping['†'] = 'a';
    mapping['6'] = 'i';
    mapping[':'] = 'l';
    mapping['0'] = 'u';
    mapping['('] = 'm';
    mapping['9'] = 'c';
    mapping['2'] = 'g';
    mapping['1'] = 'y';
    mapping['.'] = 'f';
    mapping[','] = 'p';
    mapping['?'] = 'b';
    mapping['['] = 'v';
    mapping[']'] = 'w';
    mapping['¶'] = 'x';
    mapping['—'] = 'q';
}

int main() {
    const char ciphertext[] = "53‡‡†305))6*;4826)4‡.)4‡);806*;48†8¶60))85;;]8*;:‡*8†83 (88)5*†;46(;88*96*?;8)*‡(;485);5*†2:*‡(;4956*2(5*—4)8¶8* ;4069285);)6†8)4‡‡;1(‡9;48081;8:8‡1;48†85;4)485†528806*81 (‡9;48;(88;4(‡?34;48)4‡;161;:188;‡?;";

    char output[MAX_TEXT];
    countFrequency(ciphertext);
    initializeMapping();
    applyMapping(ciphertext, output);
    printf("\n\n=== Partial Decryption Output ===\n");
    printf("%s\n", output);

    return 0;
}

