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
#include "../../shared/TypeChecking.h"
#include "../../shared/SymbolTable.h"
#include <limits.h>

/** Initialize module's internal state. */
void initializeNonixComputerModule();

/** Shutdown module's internal state. */
void shutdownNonixComputerModule();

/**
 * The result of a computation. It's considered valid only if "succeed" is
 * true.
 */
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
ComputationResult computeEvaluateStatement(EvaluateStatement * evaluateStatement, SymbolEntry * symbolTable);
ComputationResult computeAdequateStatement(AdequateStatement * adequateStatement, SymbolEntry * symbolTable);
ComputationResult computeProgram(Program * program, SymbolEntry * symbolTable);

#endif
