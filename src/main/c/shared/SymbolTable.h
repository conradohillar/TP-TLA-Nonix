#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "HashMap.h"
#include "Type.h"
#include "../frontend/syntactic-analysis/AbstractSyntaxTree.h"

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
  union {
    struct {
      int argc;                 // For SYMBOL_OPERATOR, number of arguments
      TruthTable *truth_table;  // For SYMBOL_OPERATOR, truth table
    } operator_data;
    boolean truth_value;      // For SYMBOL_VARIABLE, the truth value
    Expression *expression_node; // For SYMBOL_FORMULA, expression node
    ValuationList *valuation_list; // For SYMBOL_VALUATION, valuation list
    OpsetList *opset_list;       // For SYMBOL_OPSET, opset list
  } data;
} SymbolEntry;

// Functions to manage the table
boolean insert_symbol(SymbolEntry **table, const char *name, int argc_or_truth_value, void* data_node, SymbolType type);

SymbolEntry *find_symbol(SymbolEntry *table, const char *name);

boolean update_variable_truth_value(SymbolEntry *table, const char *name, boolean new_truth_value);
boolean update_data_node(SymbolEntry *table, const char *name, void *new_data_node);

boolean remove_symbol(SymbolEntry **table, const char *name);

void free_symbol_table(SymbolEntry **table);

#endif
