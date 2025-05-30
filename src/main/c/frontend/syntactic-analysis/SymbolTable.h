// symbol_table.h
#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "../../shared/HashMap.h"

typedef enum {
    SYMBOL_VARIABLE,
    SYMBOL_FORMULA,
    SYMBOL_VALUATION,
    SYMBOL_OPERATOR,
    SYMBOL_OPSET,
    // ...
} SymbolType;

typedef struct SymbolEntry {
    const char *name;         
    SymbolType type;          
    UT_hash_handle hh;
} SymbolEntry;

// Functions to manage the table
int insert_symbol(SymbolEntry **table, const char *name, SymbolType type);
SymbolEntry *find_symbol(SymbolEntry *table, const char *name);
void free_symbol_table(SymbolEntry **table);

#endif
