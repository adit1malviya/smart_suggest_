#include <stdio.h>
#include <string.h>
#include "utils.h"
#include "recent_searches.h"
#include "ranking.h"
#include "synonyms.h"

RecentSearches recentList;

int main() {
    initRecentList(&recentList);
    loadWords("wordlist.txt");
    loadSynonyms("thesaurus_db.txt");

    printf("=== Smart Suggest ===\nType 'exit' to quit.\n\n");

    char input[100];
    while (1) {
        printf("Type something: ");
        if (!fgets(input, sizeof(input), stdin)) break;
        input[strcspn(input, "\n")] = 0;

        cleanInput(input);
        convertToLower(input);

        if (strcmp(input, "exit") == 0) break;
        if (strlen(input) == 0) continue;

        addToRecent(&recentList, input);
        updateWordFrequency(input);

        displayMatches(input, &recentList);
        printSynonyms(input);
        printf("\n");
    }

    freeSynonymList();
    return 0;
}
