#ifndef RANKING_H
#define RANKING_H

#include "recent_searches.h"

void loadWords(const char *filename);
void saveWords(const char *filename);
void updateWordFrequency(const char *word);
void displayMatches(const char *prefix, RecentSearches *recent);

#endif
