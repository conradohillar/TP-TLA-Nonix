#include "SymbolTable.h"
#include <string.h> // Para strdup y strlen
#include <stdlib.h> // Para malloc y free

boolean insert_symbol(SymbolEntry **table, const char *name_param, int argc_or_truth_value, void *data_node, SymbolType type) {

  if(find_symbol(*table, name_param) != NULL){
    return false;
  }

  SymbolEntry *s = malloc(sizeof(SymbolEntry));
  s->name = strdup(name_param);
  s->type = type;
  switch (type) {
    case SYMBOL_OPERATOR:
      s->data.operator_data.argc = argc_or_truth_value; // Número de argumentos
      s->data.operator_data.truth_table = (TruthTable*)data_node; // Tabla de verdad
      break;
    case SYMBOL_VARIABLE:
      s->data.truth_value = (boolean)argc_or_truth_value; // Valor de verdad
      break;
    case SYMBOL_FORMULA:
      s->data.expression_node = (Expression*)data_node; // Nodo de expresión AST
      break;
    case SYMBOL_VALUATION:
      s->data.valuation_list = (ValuationList*)data_node; // Lista de valuaciones
      break;
    case SYMBOL_OPSET:
      s->data.opset_list = (OpsetList*)data_node; // Lista de operadores
      break;
  }

  HASH_ADD_KEYPTR(hh, *table, s->name, strlen(s->name), s);

  return true;
}

SymbolEntry *find_symbol(SymbolEntry *table, const char *name) {
  SymbolEntry *s;
  HASH_FIND_STR(table, name, s);
  return s;
}

boolean update_variable_truth_value(SymbolEntry *table, const char *name, boolean new_truth_value) {
  SymbolEntry *s = find_symbol(table, name);
  if (s == NULL || s->type != SYMBOL_VARIABLE) {
    return false;
  }
  s->data.truth_value = new_truth_value;
  return true;
}

boolean update_data_node(SymbolEntry *table, const char *name, void *new_data_node) {
  SymbolEntry *s = find_symbol(table, name);
  if (s == NULL) {
    return false;
  }
  switch(s->type) {
    case SYMBOL_FORMULA:
      s->data.expression_node = (Expression*)new_data_node; // Actualizar nodo de expresión AST
      break;
    case SYMBOL_VALUATION:
      s->data.valuation_list = (ValuationList*)new_data_node; // Actualizar lista de valuaciones
      break;
    case SYMBOL_OPSET:
      s->data.opset_list = (OpsetList*)new_data_node; // Actualizar lista de operadores
      break;
    default:
      return false; // No se puede actualizar para otros tipos
  }
  return true;
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