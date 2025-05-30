// symbol_table.c
#include "SymbolTable.h"

int insert_symbol(SymbolEntry **table, const char *name, SymbolType type) {
    SymbolEntry *s = find_symbol(*table, name);
    if (s != NULL) {
        // Already defined symbol -> semantic error
        return 0;
    }

    s = malloc(sizeof(SymbolEntry));
    s->name = strdup(name);
    s->type = type;
    HASH_ADD_KEYPTR(hh, *table, s->name, strlen(s->name), s);

    return 1;
}

SymbolEntry *find_symbol(SymbolEntry *table, const char *name) {
    SymbolEntry *s;
    HASH_FIND_STR(table, name, s);
    return s;
}

void free_symbol_table(SymbolEntry **table) {
    SymbolEntry *s, *tmp;
    HASH_ITER(hh, *table, s, tmp) {
        HASH_DEL(*table, s);
        free(s->name);
        free(s);
    }
}