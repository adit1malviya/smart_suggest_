#ifndef RECENT_SEARCHES_H
#define RECENT_SEARCHES_H

#define MAX_RECENT 10

typedef struct {
    char items[MAX_RECENT][100];
    int size;
} RecentSearches;

void initRecentList(RecentSearches *list);
void addToRecent(RecentSearches *list, const char *word);

#endif
