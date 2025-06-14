#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "HashMap.h"
#include "Type.h"

typedef enum {
  SYMBOL_VARIABLE,
  SYMBOL_FORMULA,
  SYMBOL_VALUATION,
  SYMBOL_OPERATOR,
  SYMBOL_OPSET,
} SymbolType;

typedef struct SymbolEntry {
  const char *name;
  SymbolType type;
  UT_hash_handle hh;
  int argc; // Optional: number of arguments for operators
} SymbolEntry;

// Functions to manage the table
boolean insert_symbol(SymbolEntry **table, const char *name, int argc, SymbolType type);

SymbolEntry *find_symbol(SymbolEntry *table, const char *name);

boolean remove_symbol(SymbolEntry **table, const char *name);

void free_symbol_table(SymbolEntry **table);

#endif
