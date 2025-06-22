#ifndef NONIX_COMPUTER_HEADER
#define NONIX_COMPUTER_HEADER

/**
 * We reuse the types from the AST for convenience, but you should separate
 * the layers of the backend and frontend using another group of
 * domain-specific models or DTOs (Data Transfer Objects).
 */
#include "../../frontend/syntactic-analysis/AbstractSyntaxTree.h"
#include "../../shared/Logger.h"
#include "../../shared/Type.h"
#include "../../shared/semantic-analysis/TypeChecking.h"
#include "../../shared/semantic-analysis/SymbolTable.h"
#include <limits.h>

/** Initialize module's internal state. */
void initializeNonixComputerModule();

/** Shutdown module's internal state. */
void shutdownNonixComputerModule();

/**
 * The result of a computation. It's considered valid only if "succeed" is
 * true.
 */

typedef struct ComputedValue {
	boolean result; 		
	struct ComputedValue *next;  
} ComputedValue;

typedef struct {
	boolean succeed;
	boolean value;
} ComputationResult;

typedef ComputationResult (*LogicalBinaryOperator)(const boolean, const boolean);

/** Logical operations. */

ComputationResult and_op(const boolean leftExpression, const boolean rightExpression);
ComputationResult or_op(const boolean leftExpression, const boolean rightExpression);
ComputationResult then_op(const boolean leftExpression, const boolean rightExpression);
ComputationResult iff_op(const boolean leftExpression, const boolean rightExpression);
ComputationResult not_op(const boolean operand);

ComputationResult computeExpression(Expression * expression, SymbolEntry * symbolTable);
ComputationResult computeCustomExpression(CustomExpression * customExpression, SymbolEntry * symbolTable);
ComputationResult computeTruthTable(TruthTable * truthTable, VariableList * variableList, SymbolEntry * symbolTable);
ComputationResult computeTruthTableFromTruthValueArray(TruthTable *truthTable, boolean *truthValueArray, int varCount);
ComputationResult computeEvaluateStatement(EvaluateStatement * evaluateStatement, SymbolEntry * symbolTable);
ComputationResult computeAdequateStatement(AdequateStatement * adequateStatement, SymbolEntry * symbolTable);
ComputedValue * computeProgram(Program * program, SymbolEntry * symbolTable, boolean * isValidProgram);

void free_results_list(ComputedValue * results_list);

#endif
