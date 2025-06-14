#include "SymbolTable.h"
#include <string.h> // Para strdup y strlen
#include <stdlib.h> // Para malloc y free

boolean insert_symbol(SymbolEntry **table, const char *name_param, int argc,
                      SymbolType type) {

  if(find_symbol(*table, name_param) != NULL){
    return false;
  }

  SymbolEntry *s = malloc(sizeof(SymbolEntry));
  s->name = strdup(name_param);
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

boolean remove_symbol(SymbolEntry **table, const char *name) {
  SymbolEntry *s = find_symbol(*table, name);
  if (s == NULL) {
    return false;
  }
  HASH_DEL(*table, s); 
  free((void *)s->name);
  free(s);
  return true; 
}

void free_symbol_table(SymbolEntry **table) {
  SymbolEntry *s, *tmp;
  HASH_ITER(hh, *table, s, tmp) {
    HASH_DEL(*table, s); 
    if (s->name) {
      free((void *)s->name); // Liberar la copia del nombre que hizo la tabla
    }
    free(s); 
  }
  *table = NULL;
}