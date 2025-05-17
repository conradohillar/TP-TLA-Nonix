#ifndef BISON_ACTIONS_HEADER
#define BISON_ACTIONS_HEADER

#include "../../shared/CompilerState.h"
#include "../../shared/Logger.h"
#include "../../shared/Type.h"
#include "AbstractSyntaxTree.h"
#include "SyntacticAnalyzer.h"
#include <stdlib.h>

/** Initialize module's internal state. */
void initializeBisonActionsModule();

/** Shutdown module's internal state. */
void shutdownBisonActionsModule();

/**
 * Bison semantic actions.
 */

TruthTable * TruthTableAction(TruthTable * table, TruthTableEntry * entry);

TruthTableEntry * TruthTableMapperEntryAction(TruthValueList * truthValueList, TruthValue * mapValue);

TruthTableEntry * TruthTableOtherwiseEntryAction(TruthValue * otherwiseValue);

TruthValueList * TruthValueListAction(TruthValueList * truthValueList, TruthValueOrWildcard * truthValueOrWildcard);

TruthValueOrWildcard * TruthValueTypeAction(TruthValue * truthValue);

TruthValueOrWildcard * WildcardTypeAction();

TruthValue * TruthValueAction(boolean value);

BinaryExpression * BinaryExpressionSemanticAction(Expression * leftExpression, Expression * rightExpression, BinaryOperatorType operatorType);

CustomExpression * PredefinedFormulaSemanticAction(PredefinedFormula predefinedFormula);

CustomExpression * CustomOperatorSemanticAction(CustomOperator * customOperator);

NotExpression * NotExpressionSemanticAction(Expression * expression);

Expression * BinaryTypeAction(BinaryExpression * binaryExpression);

Expression * CustomTypeAction(CustomExpression * customExpression);

Expression * NotTypeAction(NotExpression * notExpression);

Expression * VariableTypeAction(Variable variable);

VariableList * VariableListAction(VariableList * variableList, Variable variable);

ValuationList * ValuationListAction(ValuationList * valuationList, Valuation * valuation);

Valuation * ValuationAction(Variable variable, TruthValue * truthValue);

OpsetList * OpsetListAction(OpsetList * opsetList, Operator operator);

EvaluateStatement * EvaluateFormulaAction(const char * formulaName, const char * valuationName);

AdequateStatement * CheckAdequacyAction(const char * opsetName);

DefineVariable * DefineVariableAction(VariableList * variableList);

DefineFormula * DefineFormulaAction(const char * name, Expression * expression);

DefineValuation * DefineValuationAction(const char * name, ValuationList * valuationList);

DefineOperator * DefineOperatorAction(CustomOperator * customOperator, TruthTable * truthTable);

CustomOperator * DefineCustomOperatorAction(const char * name, VariableList * variableList);

DefineOpset * DefineOpsetAction(const char * name, OpsetList * opsetList);

Statement * DefineVariableStatementSemanticAction(DefineVariable * defineVariable);

Statement * DefineFormulaStatementSemanticAction(DefineFormula * defineFormula);

Statement * DefineValuationStatementSemanticAction(DefineValuation * defineValuation);

Statement * DefineOperatorStatementSemanticAction(DefineOperator * defineOperator);

Statement * DefineOpsetStatementSemanticAction(DefineOpset * defineOpset);

Statement * EvaluateStatementSemanticAction(EvaluateStatement * evaluateStatement);

Statement * AdequateStatementSemanticAction(AdequateStatement * adequateStatement);

Program * ProgramStatementSemanticAction(CompilerState * compilerState, Program * nextProgram, Statement * statement);


#endif
