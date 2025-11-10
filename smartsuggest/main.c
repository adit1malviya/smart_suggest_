#include <stdio.h>
#include <string.h>
#include "utils.h"
#include "recent_searches.h"
#include "ranking.h"
#include "synonyms.h"
#include "edit_distance.h"

RecentSearches recentList;

int main() {
    // Initialize recent searches list
    initRecentList(&recentList);

    // Load stored data
    loadWords("wordlist.txt");
    loadSynonyms("thesaurus_db.txt");

    printf("=== Smart Suggest ===\nType 'exit' to quit.\n\n");

    char userInput[100];
    char correctedWord[100];

    while (1) {
        printf("Type something: ");
        if (!fgets(userInput, sizeof(userInput), stdin)) break;
        userInput[strcspn(userInput, "\n")] = 0; // remove newline

        cleanInput(userInput);
        convertToLower(userInput);

        if (strcmp(userInput, "exit") == 0) break;
        if (strlen(userInput) == 0) continue;

        // Auto-correct user input based on edit distance
        getClosestWord(userInput, correctedWord);

        // If auto-corrected word is different, use it
        if (strcmp(userInput, correctedWord) != 0) {
            printf("%s\n", correctedWord);
            strcpy(userInput, correctedWord);
        }

        // Add to recent searches and update ranking
        addToRecent(&recentList, userInput);
        updateWordFrequency(userInput);

        // Show ranked word suggestions
        displayMatches(userInput, &recentList);

        // Show synonyms for the input word
        printSynonyms(userInput);
        printf("\n");
    }

    // Save updated frequencies to file
    saveWords("wordlist.txt");

    // Free allocated memory
    freeSynonymList();

    return 0;
}
