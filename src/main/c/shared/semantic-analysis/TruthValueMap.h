#ifndef TRUTH_VALUE_MAP_H
#define TRUTH_VALUE_MAP_H

#include "../HashMap.h"
#include "../Type.h"
#include "../../frontend/syntactic-analysis/AbstractSyntaxTree.h"

typedef struct TruthValueMapEntry {
    unsigned long long combination; // Unique key for the truth value combination
    UT_hash_handle hh; // For hash table handling
} TruthValueMapEntry;

// Functions to manage the table
boolean check_truth_value_entry(TruthValueMapEntry **truthValueMap, TruthValueList *truthValueList, unsigned int n_vars);
boolean insert_combination(TruthValueMapEntry **table, unsigned long long combination);
TruthValueMapEntry *find_combination(TruthValueMapEntry *table, unsigned long long combination);
unsigned int size(TruthValueMapEntry *table);
void free_truth_value_map(TruthValueMapEntry **table);



#endif
