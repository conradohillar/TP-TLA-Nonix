#include "SymbolTable.h"

boolean insert_symbol(SymbolEntry **table, const char *name, int argc,
                      SymbolType type) {
  SymbolEntry *s = find_symbol(*table, name);
  if (s != NULL) {
    // Already defined symbol -> semantic error
    return false;
  }

  s = malloc(sizeof(SymbolEntry));
  s->name = name;
  s->type = type;
  s->argc = argc;
  HASH_ADD_KEYPTR(hh, *table, s->name, strlen(s->name), s);

  return true;
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
    free(s);
  }
}