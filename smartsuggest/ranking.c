#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ranking.h"

#define MAX_WORDS 1000
#define MAX_SHOW 5

typedef struct {
    char text[100];
    int freq;
} WordInfo;

WordInfo words[MAX_WORDS];
int totalWords = 0;

int findIndex(const char *word) {
    for (int i = 0; i < totalWords; i++)
        if (strcmp(words[i].text, word) == 0)
            return i;
    return -1;
}

void loadWords(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) return;
    while (fscanf(fp, "%s %d", words[totalWords].text, &words[totalWords].freq) == 2) {
        totalWords++;
        if (totalWords >= MAX_WORDS) break;
    }
    fclose(fp);
}

void saveWords(const char *filename) {
    FILE *fp = fopen(filename, "w");
    for (int i = 0; i < totalWords; i++)
        fprintf(fp, "%s %d\n", words[i].text, words[i].freq);
    fclose(fp);
}

void updateWordFrequency(const char *word) {
    int idx = findIndex(word);
    if (idx == -1 && totalWords < MAX_WORDS) {
        strcpy(words[totalWords].text, word);
        words[totalWords].freq = 1;
        totalWords++;
    } else if (idx != -1) {
        words[idx].freq++;
    }
    saveWords("wordlist.txt");
}

int startsWith(const char *word, const char *prefix) {
    return strncmp(word, prefix, strlen(prefix)) == 0;
}

int sortByFrequency(const void *a, const void *b) {
    WordInfo *w1 = (WordInfo *)a;
    WordInfo *w2 = (WordInfo *)b;
    return w2->freq - w1->freq;
}

void displayMatches(const char *prefix, RecentSearches *recent) {
    WordInfo matched[MAX_WORDS];
    int matchCount = 0;

    for (int i = 0; i < totalWords; i++) {
        if (startsWith(words[i].text, prefix))
            matched[matchCount++] = words[i];
    }

    qsort(matched, matchCount, sizeof(WordInfo), sortByFrequency);

    for (int i = 0; i < matchCount && i < MAX_SHOW; i++)
        printf("%s\n", matched[i].text);
}
