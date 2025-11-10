#include <stdio.h>
#include <string.h>
#include <limits.h>
#include "edit_distance.h"

#define WORDLIST_FILE "wordlist.txt"
#define MAX_WORD_LEN 100

// Function to calculate Levenshtein Distance
int editDistance(const char *a, const char *b) {
    int lenA = strlen(a), lenB = strlen(b);
    int dp[MAX_WORD_LEN + 1][MAX_WORD_LEN + 1];

    for (int i = 0; i <= lenA; i++) dp[i][0] = i;
    for (int j = 0; j <= lenB; j++) dp[0][j] = j;

    for (int i = 1; i <= lenA; i++) {
        for (int j = 1; j <= lenB; j++) {
            int cost = (a[i - 1] == b[j - 1]) ? 0 : 1;
            int del = dp[i - 1][j] + 1;
            int ins = dp[i][j - 1] + 1;
            int sub = dp[i - 1][j - 1] + cost;

            int min = del < ins ? del : ins;
            if (sub < min) min = sub;

            dp[i][j] = min;
        }
    }
    return dp[lenA][lenB];
}

// Finds the closest matching word from wordlist
void getClosestWord(const char *input, char *correctedWord) {
    FILE *fp = fopen(WORDLIST_FILE, "r");
    if (!fp) {
        strcpy(correctedWord, input);
        return;
    }

    char word[MAX_WORD_LEN];
    int minDist = INT_MAX;
    char bestWord[MAX_WORD_LEN] = "";

    while (fscanf(fp, "%s", word) != EOF) {
        int freq;
        fscanf(fp, "%d", &freq);  // skip frequency
        int dist = editDistance(input, word);
        if (dist < minDist) {
            minDist = dist;
            strcpy(bestWord, word);
        }
    }
    fclose(fp);

    // Auto-correct only if edit distance <= 2
    if (minDist <= 2 && strlen(bestWord) > 0)
        strcpy(correctedWord, bestWord);
    else
        strcpy(correctedWord, input);
}
