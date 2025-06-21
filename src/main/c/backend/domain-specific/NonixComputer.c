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
 * A binary operator that always returns an invalid computation result.
 */
static ComputationResult _invalidOperator(const boolean leftExpression,
                                          const boolean rightExpression) {
  return _invalidComputation();
}

/**
 * A computation that always returns an invalid result.
 */
static ComputationResult _invalidComputation() {
  ComputationResult computationResult = {.succeed = false, .value = 0};
  return computationResult;
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

  // Array temporal para los valores de verdad de las variables de la instancia
  // del operador.
  boolean operator_var_truth_values[num_vars_in_operator];

  // Se recorre la variableList y se obtienen los valores de verdad de la tabla
  // de símbolos.
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

  // Si no se encontró ninguna fila que coincida.
  if (!result.succeed) {
    logError(
        _logger,
        "No matching row found in truth table for the given variable values.");
    return _invalidComputation();
  }
  // Si se llegó hasta acá es por una entrada de tipo OTHERWISE, se devuelve su
  // valor.
  return result;
}

ComputationResult computeTruthTableFromTruthValueArray(TruthTable *truthTable,
                                                       boolean *truthValueArray,
                                                       int varCount) {
  ComputationResult result = {.succeed = false, .value = false};

  // Se recorre cada entrada de la truthTable.
  TruthTable *current_row = truthTable;
  while (current_row != NULL) {
    switch (current_row->entry->type) {
    case TRUTH_VALUE_LIST: {
      // Se comparan los valores de verdad de la entrada actual con
      // truthValueArray.
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

      if (match) {
        result.succeed = true;
        result.value =
            current_row->entry->mapValue
                ->value; // Asignar el valor de verdad correspondiente.
        return result;   // Se encontró la fila que coincide.
      }
      break;
    }
    case OTHERWISE_ENTRY: {
      // Si se llega a una entrada de tipo OTHERWISE, se guarda su valor en caso
      // de que haya otra entrada más específica.
      result.succeed = true;
      result.value = current_row->entry->otherwiseValue->value;
      break;
    }
    default:
      logError(_logger, "Invalid TruthTableEntryType: %d",
               current_row->entry->type);
      return _invalidComputation();
    }
    // Avanzar a la siguiente fila de la truthTable.
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
  // 1. Buscar la valuación en la tabla de símbolos.
  SymbolEntry *symbol =
      find_symbol(symbolTable, evaluateStatement->valuationName);
  if (symbol == NULL || symbol->type != SYMBOL_VALUATION) {
    logError(_logger, "Valuation '%s' is not defined.",
             evaluateStatement->valuationName);
    return _invalidComputation();
  }

  // 2. Actualizar los valores de verdad de las variables según la valuación.
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

  // 3. Buscar la fórmula en la tabla de símbolos.
  symbol = find_symbol(symbolTable, evaluateStatement->formulaName);
  if (symbol == NULL || symbol->type != SYMBOL_FORMULA) {
    logError(_logger, "Formula '%s' is not defined.",
             evaluateStatement->formulaName);
    return _invalidComputation();
  }

  // 4. Calcular el resultado de la expresión asociada a la fórmula,
  // correspondiente a los valores de la valuación.
  return computeExpression(symbol->data.expression_node, symbolTable);
}

ComputationResult computeAdequateStatement(AdequateStatement *adequateStatement,
                                           SymbolEntry *symbolTable) {
  // 1. Buscar el opset en la tabla de símbolos.
  SymbolEntry *symbol = find_symbol(symbolTable, adequateStatement->opsetName);
  if (symbol == NULL || symbol->type != SYMBOL_OPSET) {
    logError(_logger, "Opset '%s' is not defined.",
             adequateStatement->opsetName);
    return _invalidComputation();
  }
  OpsetList *opset = symbol->data.opset_list;
  if (isFunctionallyComplete(opset, symbolTable)) {
    // TODO: VER QUE HAY QUE HACER CON EL RESULTADO ???
  }

  return (ComputationResult){
      .succeed = true,
      .value = 1}; // Return an invalid computation result for now.
}

ComputationResult computeProgram(Program *program, SymbolEntry *symbolTable) {
  ComputationResult result = {.succeed = true, .value = 1};

  Program *currentProgram = program;
  while (currentProgram != NULL) {
    switch (currentProgram->statement->type) {
    case EVALUATE_STATEMENT: {
      return computeEvaluateStatement(
          currentProgram->statement->evaluateStatement, symbolTable);
    }
    case ADEQUATE_STATEMENT: {
      return computeAdequateStatement(
          currentProgram->statement->adequateStatement, symbolTable);
    }
    default:
      result.succeed = true;
    }
    currentProgram = currentProgram->next;
  }
  // if(!result.succeed) {
  // 	logError(_logger, "No valid computation statement found in the
  // program."); 	return _invalidComputation();
  // }
  return result;
}