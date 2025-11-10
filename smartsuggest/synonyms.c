#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "synonyms.h"

SynonymNode *synHead = NULL;

void loadSynonyms(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) return;

    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        line[strcspn(line, "\n")] = 0;

        char *mainWord = strtok(line, ":");
        char *synLine = strtok(NULL, ":");
        if (!mainWord || !synLine) continue;

        SynonymNode *node = malloc(sizeof(SynonymNode));
        strcpy(node->baseWord, mainWord);
        node->total = 0;
        node->next = synHead;
        synHead = node;

        char *token = strtok(synLine, " ");
        while (token && node->total < 10) {
            strcpy(node->related[node->total++], token);
            token = strtok(NULL, " ");
        }
    }

    fclose(fp);
}

void printSynonyms(const char *word) {
    SynonymNode *ptr = synHead;
    while (ptr) {
        if (strcmp(ptr->baseWord, word) == 0) {
            for (int i = 0; i < ptr->total; i++)
                printf("%s\n", ptr->related[i]);
            return;
        }
        ptr = ptr->next;
    }
}

void freeSynonymList() {
    SynonymNode *temp = synHead;
    while (temp) {
        SynonymNode *next = temp->next;
        free(temp);
        temp = next;
    }
}
