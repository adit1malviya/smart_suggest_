#include <stdio.h>
#include <string.h>
#include "recent_searches.h"

void initRecentList(RecentSearches *list) {
    list->size = 0;
}

void addToRecent(RecentSearches *list, const char *word) {
    for (int i = 0; i < list->size; i++) {
        if (strcmp(list->items[i], word) == 0)
            return; // already added
    }

    if (list->size < MAX_RECENT) {
        strcpy(list->items[list->size++], word);
    } else {
        for (int i = 1; i < MAX_RECENT; i++)
            strcpy(list->items[i - 1], list->items[i]);
        strcpy(list->items[MAX_RECENT - 1], word);
    }
}
