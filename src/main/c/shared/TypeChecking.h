#ifndef TYPE_CHECKING_HEADER
#define TYPE_CHECKING_HEADER

#include "../frontend/syntactic-analysis/AbstractSyntaxTree.h"
#include "SymbolTable.h"
#include "../frontend/syntactic-analysis/SyntacticAnalyzer.h"

unsigned int CheckTypeProgram(Program *program);

unsigned int CheckTypeStatement(Statement *statement);

unsigned int CheckTypeExpression(Expression *expression);

unsigned int CheckTypeNotExpression(NotExpression *notExpression);

unsigned int CheckTypeBinaryExpression(BinaryExpression *binaryExpression);

unsigned int CheckTypeCustomExpression(CustomExpression *customExpression);

unsigned int CheckTypeDefineVariable(DefineVariable *defineVariable);

unsigned int CheckTypeVariableList(VariableList *variableList);

unsigned int CheckTypeDefineFormula(DefineFormula *defineFormula);

unsigned int CheckTypeDefineValuation(DefineValuation *defineValuation);

unsigned int CheckTypeValuationList(ValuationList *valuationList);

unsigned int CheckTypeValuation(Valuation *valuation);

unsigned int CheckTypeDefineOpset(DefineOpset *defineOpset);

unsigned int CheckTypeOpsetList(OpsetList *opsetList);

unsigned int CheckTypeDefineOperator(DefineOperator *defineOperator);

unsigned int CheckTypeCustomOperator(CustomOperator *customOperator);

unsigned int CheckTypeAdequateStatement(AdequateStatement *adequateStatement);

unsigned int CheckTypeEvaluateStatement(EvaluateStatement *evaluateStatement);

unsigned int CheckTypeTruthTable(TruthTable *truthTable, unsigned int args);

unsigned int CheckTypeTruthTableEntry(TruthTableEntry *truthTableEntry, unsigned int args);

unsigned int CheckTypeTruthValueList(TruthValueList *truthValueList);

unsigned int CheckTypeTruthValueOrWildcard(TruthValueOrWildcard *truthValueOrWildcard);

unsigned int CheckTypeTruthValue(TruthValue *truthValue);

unsigned int CheckTypeVariable(Variable variable);

unsigned int CheckTypeOpsetName(const char *opsetName);

unsigned int CheckTypeFormula(const char *formulaName);

unsigned int CheckTypeOperator(Operator operatorName, unsigned int argc);

unsigned int CheckTypeValuationName(const char *valuationName);

unsigned int list_size(void * list);

#endif