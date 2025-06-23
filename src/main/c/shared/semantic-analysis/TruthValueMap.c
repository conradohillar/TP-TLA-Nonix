#include "TruthValueMap.h"

static void truth_value_list_to_keys(const TruthValueList *list, unsigned long long key_prefix, int bit_pos, unsigned long long *out_keys, int *out_count, unsigned int max_keys);

static void truth_value_list_to_keys(const TruthValueList *list, unsigned long long key_prefix, int bit_pos, unsigned long long *out_keys, int *out_count, unsigned int max_keys) {
    if (list == NULL) {
        // Reached the end of the list, save the generated key
        if (*out_count < max_keys) {
            out_keys[*out_count] = key_prefix;
            (*out_count)++;
        }
        return;
    }
    if (list->truthValueOrWildcard->type == WILDCARD_VALUE) {
        // Expand with 0 (false)
        truth_value_list_to_keys(list->next, key_prefix, bit_pos + 1, out_keys, out_count, max_keys);
        // Expand with 1 (true)
        truth_value_list_to_keys(list->next, key_prefix | (1ULL << bit_pos), bit_pos + 1, out_keys, out_count, max_keys);
    } else {
        int bit = list->truthValueOrWildcard->truthValue->value ? 1 : 0;
        truth_value_list_to_keys(list->next, key_prefix | ((unsigned long long)bit << bit_pos), bit_pos + 1, out_keys, out_count, max_keys);
    }
}

/* PUBLIC FUNCTIONS */
boolean check_truth_value_entry(TruthValueMapEntry **truthValueMap, TruthValueList *truthValueList, unsigned int n_vars) {
    if (truthValueList == NULL) {
        return false; 
    }
    unsigned int max_keys = 1 << n_vars; // 2^n_vars
    unsigned long long *keys = malloc(max_keys * sizeof(unsigned long long));
    int count = 0;
    truth_value_list_to_keys(truthValueList, 0, 0, keys, &count, max_keys);
    
    for(int i = 0; i < count; i++) {
        if (!insert_combination(truthValueMap, keys[i])) {
            free(keys);
            return false;  // Invalid entry, already exists
        }
    }
    free(keys);
    return true;
}

boolean insert_combination(TruthValueMapEntry **table, unsigned long long combination) {
    if(find_combination(*table, combination) != NULL){
        return false;
    }
    TruthValueMapEntry *entry = malloc(sizeof(TruthValueMapEntry));
    entry->combination = combination;
    HASH_ADD(hh, *table, combination, sizeof(entry->combination), entry);
    return true;
}

TruthValueMapEntry *find_combination(TruthValueMapEntry *table, unsigned long long combination) {
    TruthValueMapEntry *entry;
    HASH_FIND(hh, table, &combination, sizeof(combination), entry);
    return entry;
}

unsigned int size(TruthValueMapEntry *table) {
    return HASH_COUNT(table);
}

void free_truth_value_map(TruthValueMapEntry **table) {
    TruthValueMapEntry *current_entry, *tmp;
    HASH_ITER(hh, *table, current_entry, tmp) {
        HASH_DEL(*table, current_entry);
        free(current_entry);
    }
    *table = NULL;
}


