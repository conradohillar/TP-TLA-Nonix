#include "NonixComputer.h"
#include "PostFunctionalCompleteness.h"

/* MODULE INTERNAL STATE */

static Logger *_logger = NULL;

void initializeNonixComputerModule() {
  _logger = createLogger("NonixComputer");
}

void shutdownNonixComputerModule() {
  if (_logger != NULL) {
    destroyLogger(_logger);
  }
}

/** PRIVATE FUNCTIONS */

static LogicalBinaryOperator
_expressionTypeToBinaryOperator(const BinaryOperatorType type);
static boolean
computeTruthValueOrWildcard(const TruthValueOrWildcard *truthValueOrWildcard);
static ComputationResult _invalidComputation();
static ComputationResult _invalidOperator(const boolean leftExpression,
                                          const boolean rightExpression);
static ComputedValue *_addEvaluateResult(ComputedValue *list, boolean result, ResultType type, EvaluateStatement *evaluateStatement);
static ComputedValue *_addAdequateResult(ComputedValue *list, boolean result, ResultType type, AdequateStatement *adequateStatement);

/**
 * Converts and expression type to the proper binary operator. If that's not
 * possible, returns a binary operator that always returns an invalid
 * computation result.
 */
static LogicalBinaryOperator
_expressionTypeToBinaryOperator(const BinaryOperatorType type) {
  switch (type) {
  case BINOP_AND:
    return and_op;
  case BINOP_OR:
    return or_op;
  case BINOP_THEN:
    return then_op;
  case BINOP_IFF:
    return iff_op;
  default:
    logError(_logger, "The specified operator type is invalid: %d", type);
    return _invalidOperator;
  }
}
/**
 * A computation that always returns an invalid result.
 */
static ComputationResult _invalidOperator(const boolean leftExpression,
                                          const boolean rightExpression) {
  return _invalidComputation();
}

static ComputationResult _invalidComputation() {
  ComputationResult computationResult = {.succeed = false, .value = 0};
  return computationResult;
}

static ComputedValue *_addEvaluateResult(ComputedValue *list, boolean result, ResultType type, EvaluateStatement *evaluateStatement) {
  if (list == NULL) {
    // If the list is empty, create a new node.
    ComputedValue *newNode = (ComputedValue *)malloc(sizeof(ComputedValue));
    if (newNode == NULL) {
      logError(_logger, "Memory allocation failed for ComputedValue.");
      return NULL;
    }
    newNode->result = result;
    newNode->type = type;
    newNode->evaluateStatement = evaluateStatement;
    newNode->next = NULL;
    return newNode;
  }
  list->next = _addEvaluateResult(list->next, result, type, evaluateStatement);
  return list;
}

static ComputedValue *_addAdequateResult(ComputedValue *list, boolean result, ResultType type, AdequateStatement *adequateStatement) {
  if (list == NULL) {
    // If the list is empty, create a new node.
    ComputedValue *newNode = (ComputedValue *)malloc(sizeof(ComputedValue));
    if (newNode == NULL) {
      logError(_logger, "Memory allocation failed for ComputedValue.");
      return NULL;
    }
    newNode->result = result;
    newNode->type = type;
    newNode->adequateStatement = adequateStatement;
    newNode->next = NULL;
    return newNode;
  }
  list->next = _addAdequateResult(list->next, result, type, adequateStatement);
  return list;
}

/** PUBLIC FUNCTIONS */

ComputationResult and_op(const boolean leftExpression,
                         const boolean rightExpression) {
  ComputationResult computationResult = {
      .succeed = true, .value = leftExpression && rightExpression};
  return computationResult;
}

ComputationResult or_op(const boolean leftExpression,
                        const boolean rightExpression) {
  ComputationResult computationResult = {
      .succeed = true, .value = leftExpression || rightExpression};
  return computationResult;
}

ComputationResult then_op(const boolean leftExpression,
                          const boolean rightExpression) {
  ComputationResult computationResult = {
      .succeed = true, .value = !leftExpression || rightExpression};
  return computationResult;
}

ComputationResult iff_op(const boolean leftExpression,
                         const boolean rightExpression) {
  ComputationResult computationResult = {
      .succeed = true, .value = (leftExpression == rightExpression)};
  return computationResult;
}

ComputationResult not_op(const boolean expression) {
  ComputationResult computationResult = {.succeed = true, .value = !expression};
  return computationResult;
}

ComputationResult computeExpression(Expression *expression,
                                    SymbolEntry *symbolTable) {
  switch (expression->type) {
  case NOT_EXPRESSION: {
    NotExpression *notExpression = expression->notExpression;
    ComputationResult result =
        computeExpression(notExpression->expression, symbolTable);
    if (!result.succeed) {
      return result;
    }
    return not_op(result.value);
  }
  case BINARY_EXPRESSION: {
    BinaryExpression *binaryExpression = expression->binaryExpression;
    ComputationResult leftResult =
        computeExpression(binaryExpression->leftExpression, symbolTable);
    if (!leftResult.succeed) {
      return leftResult;
    }
    const ComputationResult rightResult =
        computeExpression(binaryExpression->rightExpression, symbolTable);
    if (!rightResult.succeed) {
      return rightResult;
    }
    LogicalBinaryOperator operatorFunction =
        _expressionTypeToBinaryOperator(binaryExpression->operatorType);
    return operatorFunction(leftResult.value, rightResult.value);
  }
  case CUSTOM_EXPRESSION: {
    return computeCustomExpression(expression->customExpression, symbolTable);
  }
  case VARIABLE_EXPRESSION: {
    SymbolEntry *symbol = find_symbol(symbolTable, expression->variable);
    if (symbol == NULL) {
      logError(_logger, "Variable '%s' is not defined.", expression->variable);
      return _invalidComputation();
    }
    const ComputationResult result = {.succeed = true,
                                      .value = symbol->data.truth_value};
    return result;
  }
  default:
    logError(_logger, "The expression type is invalid: %d", expression->type);
    return _invalidComputation();
  }
}

ComputationResult computeCustomExpression(CustomExpression *customExpression,
                                          SymbolEntry *symbolTable) {
  switch (customExpression->type) {
  case PREDEFINED_FORMULA: {
    SymbolEntry *symbol =
        find_symbol(symbolTable, customExpression->predefinedFormula);
    if (symbol == NULL) {
      logError(_logger, "Predefined formula '%s' is not defined.",
               customExpression->predefinedFormula);
      return _invalidComputation();
    }
    const ComputationResult result =
        computeExpression(symbol->data.expression_node, symbolTable);
    return result;
  }
  case CUSTOM_OPERATOR: {
    SymbolEntry *symbol =
        find_symbol(symbolTable, customExpression->customOperator->name);
    if (symbol == NULL || symbol->type != SYMBOL_OPERATOR) {
      logError(_logger, "Custom operator '%s' is not defined.",
               customExpression->customOperator->name);
      return _invalidComputation();
    }
    const ComputationResult result = computeTruthTable(
        symbol->data.operator_data.truth_table,
        customExpression->customOperator->variableList, symbolTable);
    return result;
  }
  default:
    logError(_logger, "The custom expression type is invalid: %d",
             customExpression->type);
    return _invalidComputation();
  }
}

ComputationResult computeTruthTable(TruthTable *truthTable,
                                    VariableList *variableList,
                                    SymbolEntry *symbolTable) {
  int num_vars_in_operator = list_size(variableList);

  // Temporary array to store the truth values of the variables in the operator.
  // This is used to compare against the truth table entries.
  boolean operator_var_truth_values[num_vars_in_operator];

  // Traverse the variableList and obtain the truth values from the symbol table.
  VariableList *current_var_node = variableList;
  for (int i = 0; i < num_vars_in_operator && current_var_node != NULL; ++i) {
    SymbolEntry *symbol = find_symbol(symbolTable, current_var_node->variable);
    if (symbol == NULL || symbol->type != SYMBOL_VARIABLE) {
      logError(_logger, "Variable '%s' not found in symbol table",
               current_var_node->variable);
      return _invalidComputation();
    }
    operator_var_truth_values[i] = symbol->data.truth_value;
    current_var_node = current_var_node->next;
  }

  ComputationResult result = computeTruthTableFromTruthValueArray(
      truthTable, operator_var_truth_values, num_vars_in_operator);

  // If no matching row was found.
  if (!result.succeed) {
    logError(
        _logger,
        "No matching row found in truth table for the given variable values.");
    return _invalidComputation();
  }

  return result;
}

ComputationResult computeTruthTableFromTruthValueArray(TruthTable *truthTable,
                                                       boolean *truthValueArray,
                                                       int varCount) {
  ComputationResult result = {.succeed = false, .value = false};

  // Traverse each entry in the truthTable.
  TruthTable *current_row = truthTable;
  while (current_row != NULL) {
    switch (current_row->entry->type) {
    case TRUTH_VALUE_LIST: {
      // Compare the truth values of the current entry with those in the truthValueArray
      boolean match = true;
      TruthValueList *entry_val_node = current_row->entry->truthValueList;
      for (int i = 0; i < varCount; ++i) {
        if (computeTruthValueOrWildcard(entry_val_node->truthValueOrWildcard) !=
            truthValueArray[i]) {
          match = false;
          break;
        }
        entry_val_node = entry_val_node->next;
      }

      if (match) { // Found matching row.
        result.succeed = true;
        result.value =
            current_row->entry->mapValue
                ->value; // Assign corresponding truth value.
        return result;
      }
      break;
    }
    case OTHERWISE_ENTRY: {
      // If it's OTHERWISE, save its value in case there's another entry that is more specific
      result.succeed = true;
      result.value = current_row->entry->otherwiseValue->value;
      break;
    }
    default:
      logError(_logger, "Invalid TruthTableEntryType: %d",
               current_row->entry->type);
      return _invalidComputation();
    }
    // Move to the next row in the truthTable.
    current_row = current_row->next;
  }
  return result;
}

static boolean
computeTruthValueOrWildcard(const TruthValueOrWildcard *truthValueOrWildcard) {
  switch (truthValueOrWildcard->type) {
  case TRUTH_VALUE: {
    return truthValueOrWildcard->truthValue->value;
  }
  case WILDCARD_VALUE: {
    // A wildcard does not have a specific value, so we return true.
    return true;
  }
  default:
    logError(_logger, "Invalid TruthValueOrWildcard type: %d",
             truthValueOrWildcard->type);
    return false; // Default case for safety
  }
}

ComputationResult computeEvaluateStatement(EvaluateStatement *evaluateStatement,
                                           SymbolEntry *symbolTable) {
  // Search the valuation in the symbol table.
  SymbolEntry *symbol =
      find_symbol(symbolTable, evaluateStatement->valuationName);
  if (symbol == NULL || symbol->type != SYMBOL_VALUATION) {
    logError(_logger, "Valuation '%s' is not defined.",
             evaluateStatement->valuationName);
    return _invalidComputation();
  }

  // Update the truth values of the variables according to the valuation.
  ValuationList *current_valuation = symbol->data.valuation_list;
  while (current_valuation != NULL) {
    if (!update_variable_truth_value(
            symbolTable, current_valuation->valuation->variable,
            current_valuation->valuation->truthValue->value)) {
      logError(_logger, "Failed to update variable '%s' with valuation '%d'.",
               current_valuation->valuation->variable,
               current_valuation->valuation->truthValue->value);
      return _invalidComputation();
    }
    current_valuation = current_valuation->next;
  }

  // Search the formula in the symbol table.
  symbol = find_symbol(symbolTable, evaluateStatement->formulaName);
  if (symbol == NULL || symbol->type != SYMBOL_FORMULA) {
    logError(_logger, "Formula '%s' is not defined.",
             evaluateStatement->formulaName);
    return _invalidComputation();
  }

  // Calculate the result of the expression associated with the formula,
  // corresponding to the values of the valuation.
  return computeExpression(symbol->data.expression_node, symbolTable);
}

ComputationResult computeAdequateStatement(AdequateStatement *adequateStatement,
                                           SymbolEntry *symbolTable) {
  // Search the opset in the symbol table.
  SymbolEntry *symbol = find_symbol(symbolTable, adequateStatement->opsetName);
  if (symbol == NULL || symbol->type != SYMBOL_OPSET) {
    logError(_logger, "Opset '%s' is not defined.",
             adequateStatement->opsetName);
    return _invalidComputation();
  }
  OpsetList *opset = symbol->data.opset_list;

  return (ComputationResult){
      .succeed = true, .value = isFunctionallyComplete(opset, symbolTable)};
}

ComputedValue *computeProgram(Program *program, SymbolEntry *symbolTable,
                              boolean *isValidProgram) {
  ComputedValue *results_list = NULL;

  Program *currentProgram = program;
  while (currentProgram != NULL) {
    switch (currentProgram->statement->type) {

    case EVALUATE_STATEMENT: {
      ComputationResult result = computeEvaluateStatement(
          currentProgram->statement->evaluateStatement, symbolTable);
      if (!result.succeed) {
        logError(_logger, "Failed to compute evaluate statement.");
        *isValidProgram = false; // Mark the program as invalid.
        return NULL;
      }
      results_list = _addEvaluateResult(results_list, result.value, EVALUATE_RESULT, currentProgram->statement->evaluateStatement);
      break;
    }

    case ADEQUATE_STATEMENT: {
      ComputationResult result = computeAdequateStatement(
          currentProgram->statement->adequateStatement, symbolTable);
      if (!result.succeed) {
        logError(_logger, "Failed to compute adequate statement.");
        *isValidProgram = false; // Mark the program as invalid.
        return NULL;
      }
      results_list = _addAdequateResult(results_list, result.value, ADEQUATE_RESULT, currentProgram->statement->adequateStatement);
      break;
    }
    default:
      break;
    }
    currentProgram = currentProgram->next;
  }
  return results_list;
}

void free_results_list(ComputedValue *list) {
  if (list == NULL) {
    return;
  }
  free_results_list(list->next);
  free(list);
}