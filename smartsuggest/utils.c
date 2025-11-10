#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "utils.h"

// remove all symbols, digits etc.
void cleanInput(char *text) {
    char temp[100];
    int j = 0;
    for (int i = 0; text[i]; i++) {
        if (isalpha(text[i])) {
            temp[j++] = text[i];
        }
    }
    temp[j] = '\0';
    strcpy(text, temp);
}

// convert all letters to lowercase
void convertToLower(char *text) {
    for (int i = 0; text[i]; i++) {
        text[i] = tolower((unsigned char)text[i]);
    }
}
