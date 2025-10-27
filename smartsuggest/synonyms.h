#ifndef SYNONYMS_H
#define SYNONYMS_H

typedef struct SynonymNode {
    char baseWord[100];
    char related[10][100];
    int total;
    struct SynonymNode *next;
} SynonymNode;

void loadSynonyms(const char *filename);
void printSynonyms(const char *word);
void freeSynonymList();

#endif
