#ifndef POST_FUNCTIONAL_COMPLETENESS_H
#define POST_FUNCTIONAL_COMPLETENESS_H

#include "NonixComputer.h"

/**
 *  Verifies if the given set of operators is functionally complete according to
 *  Post's Functional Completeness Theorem, which states that "a set X of truth
 *  functions (of 2-valued (boolean) logic) is functionally complete if and only
 *  if for each of the five defined classes, there is a member of X which does
 *  not belong to that class".
 */
boolean isFunctionallyComplete(OpsetList *opset, SymbolEntry *symbolTable);

/**
 *  Functions closed under T
 *
 *  Verifies if the given operator is of type 1, meaning that given an operator
 *  f, f(T,T,... T) = T In other words, the operator "preserves T".
 */
boolean isType1(TruthTable *truthTable, SymbolEntry *symbolTable, int varCount);

/**
 *  Functions closed under F
 *
 *  Verifies if the given operator is of type 2, meaning that given an operator
 *  f, f(F,F,... F) = F In other words, the operator "preserves F".
 */
boolean isType2(TruthTable *truthTable, SymbolEntry *symbolTable, int varCount);

/**
 *  Counting functions
 *
 *  Verifies if the given operator is of type 3, meaning that it is a counting
 *  function. Count how many inputs are true.
 *
 *  If all rows with even number of trues give true, and all rows with odd
 *  number of trues give false, then it's an even-counting function.
 *  Or vice versa (odd=true, even=false): then it's an odd-counting function.
 *
 *  If there's any inconsistency, it’s not a Type 3 function.
 */
boolean isType3(TruthTable *truthTable, SymbolEntry *symbolTable, int varCount);

/**
 *  Monotonic functions
 *
 *  Verifies if the given operator is of type 4, meaning that given an operator
 *  f, if <x1, ... xn> and <y1, ... yn> are two rows of the truth table, and if
 *  <x1, ... xn> <= <y1, ... yn>, then f(x1, ... xn) <= f(y1, ... yn). We
 *  consider that F < T.
 */
boolean isType4(TruthTable *truthTable, SymbolEntry *symbolTable, int varCount);

/**
 *  Self-dual functions
 *
 *  Verifies if the given operator is of type 5, meaning that given an operator
 *  f, for every row of the truth table <x1, x2, ... xn>, the operator f(x1, x2,
 *  ... xn) = ¬f(¬x1, ¬x2, ... ¬xn).
 */
boolean isType5(TruthTable *truthTable, SymbolEntry *symbolTable, int varCount);

#endif