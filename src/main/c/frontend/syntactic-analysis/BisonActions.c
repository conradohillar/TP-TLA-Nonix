#include "BisonActions.h"

/* MODULE INTERNAL STATE */

static Logger *_logger = NULL;

void initializeBisonActionsModule() { _logger = createLogger("BisonActions"); }

void shutdownBisonActionsModule() {
  if (_logger != NULL) {
    destroyLogger(_logger);
  }
}

/** IMPORTED FUNCTIONS */

extern unsigned int flexCurrentContext(void);

/* PRIVATE FUNCTIONS */

static void _logSyntacticAnalyzerAction(const char *functionName);

/**
 * Logs a syntactic-analyzer action in DEBUGGING level.
 */
static void _logSyntacticAnalyzerAction(const char *functionName) {
  logDebugging(_logger, "%s", functionName);
}

/* PUBLIC FUNCTIONS */

TruthTable *TruthTableAction(TruthTable *table, TruthTableEntry *entry) {
  _logSyntacticAnalyzerAction(__FUNCTION__);
  TruthTable *truthTable = calloc(1, sizeof(TruthTable));
  truthTable->entry = entry;
  truthTable->next = table;
  return truthTable;
}

TruthTableEntry *TruthTableMapperEntryAction(TruthValueList *truthValueList,
                                             TruthValue *mapValue) {
  _logSyntacticAnalyzerAction(__FUNCTION__);
  TruthTableEntry *truthTableEntry = calloc(1, sizeof(TruthTableEntry));
  truthTableEntry->truthValueList = truthValueList;
  truthTableEntry->mapValue = mapValue;
  truthTableEntry->type = TRUTH_VALUE_LIST;
  return truthTableEntry;
}

TruthTableEntry *TruthTableOtherwiseEntryAction(TruthValue *otherwiseValue) {
  _logSyntacticAnalyzerAction(__FUNCTION__);
  TruthTableEntry *truthTableEntry = calloc(1, sizeof(TruthTableEntry));
  truthTableEntry->otherwiseValue = otherwiseValue;
  truthTableEntry->type = OTHERWISE_ENTRY;
  return truthTableEntry;
}

TruthValueList *
TruthValueListAction(TruthValueList *truthValueList,
                     TruthValueOrWildcard *truthValueOrWildcard) {
  _logSyntacticAnalyzerAction(__FUNCTION__);
  TruthValueList *newTruthValueList = calloc(1, sizeof(TruthValueList));
  newTruthValueList->truthValueOrWildcard = truthValueOrWildcard;
  newTruthValueList->next = truthValueList;
  return newTruthValueList;
}

TruthValueOrWildcard *TruthValueTypeAction(TruthValue *truthValue) {
  _logSyntacticAnalyzerAction(__FUNCTION__);
  TruthValueOrWildcard *truthValueOrWildcard =
      calloc(1, sizeof(TruthValueOrWildcard));
  truthValueOrWildcard->truthValue = truthValue;
  truthValueOrWildcard->type = TRUTH_VALUE;
  return truthValueOrWildcard;
}

TruthValueOrWildcard *WildcardTypeAction() {
  _logSyntacticAnalyzerAction(__FUNCTION__);
  TruthValueOrWildcard *truthValueOrWildcard =
      calloc(1, sizeof(TruthValueOrWildcard));
  truthValueOrWildcard->type = WILDCARD_VALUE;
  return truthValueOrWildcard;
}

TruthValue *TruthValueAction(boolean value) {
  _logSyntacticAnalyzerAction(__FUNCTION__);
  TruthValue *newTruthValue = calloc(1, sizeof(TruthValue));
  newTruthValue->value = value;
  return newTruthValue;
}

BinaryExpression *
BinaryExpressionSemanticAction(Expression *leftExpression,
                               Expression *rightExpression,
                               BinaryOperatorType operatorType) {
  _logSyntacticAnalyzerAction(__FUNCTION__);
  BinaryExpression *binaryExpression = calloc(1, sizeof(BinaryExpression));
  binaryExpression->leftExpression = leftExpression;
  binaryExpression->rightExpression = rightExpression;
  binaryExpression->operatorType = operatorType;
  return binaryExpression;
}

CustomExpression *
PredefinedFormulaSemanticAction(PredefinedFormula predefinedFormula) {
  _logSyntacticAnalyzerAction(__FUNCTION__);
  CustomExpression *customExpression = calloc(1, sizeof(CustomExpression));
  customExpression->type = PREDEFINED_FORMULA;
  customExpression->predefinedFormula = predefinedFormula;
  return customExpression;
}

CustomExpression *CustomOperatorSemanticAction(CustomOperator *customOperator) {
  _logSyntacticAnalyzerAction(__FUNCTION__);
  CustomExpression *customExpression = calloc(1, sizeof(CustomExpression));
  customExpression->type = CUSTOM_OPERATOR;
  customExpression->customOperator = customOperator;
  return customExpression;
}

NotExpression *NotExpressionSemanticAction(Expression *expression) {
  _logSyntacticAnalyzerAction(__FUNCTION__);
  NotExpression *notExpression = calloc(1, sizeof(NotExpression));
  notExpression->expression = expression;
  return notExpression;
}

Expression *BinaryTypeAction(BinaryExpression *binaryExpression) {
  _logSyntacticAnalyzerAction(__FUNCTION__);
  Expression *expression = calloc(1, sizeof(Expression));
  expression->binaryExpression = binaryExpression;
  expression->type = BINARY_EXPRESSION;
  return expression;
}

Expression *CustomTypeAction(CustomExpression *customExpression) {
  _logSyntacticAnalyzerAction(__FUNCTION__);
  Expression *expression = calloc(1, sizeof(Expression));
  expression->customExpression = customExpression;
  expression->type = CUSTOM_EXPRESSION;
  return expression;
}

Expression *NotTypeAction(NotExpression *notExpression) {
  _logSyntacticAnalyzerAction(__FUNCTION__);
  Expression *expression = calloc(1, sizeof(Expression));
  expression->notExpression = notExpression;
  expression->type = NOT_EXPRESSION;
  return expression;
}

Expression *VariableTypeAction(Variable variable) {
  _logSyntacticAnalyzerAction(__FUNCTION__);
  Expression *expression = calloc(1, sizeof(Expression));
  expression->variable = variable;
  expression->type = VARIABLE_EXPRESSION;
  return expression;
}

VariableList *VariableListAction(VariableList *variableList,
                                 Variable variable) {
  _logSyntacticAnalyzerAction(__FUNCTION__);
  VariableList *newVariableList = calloc(1, sizeof(VariableList));
  newVariableList->variable = variable;
  newVariableList->next = variableList;
  return newVariableList;
}

ValuationList *ValuationListAction(ValuationList *valuationList,
                                   Valuation *valuation) {
  _logSyntacticAnalyzerAction(__FUNCTION__);
  ValuationList *newValuationList = calloc(1, sizeof(ValuationList));
  newValuationList->valuation = valuation;
  newValuationList->next = valuationList;
  return newValuationList;
}

Valuation *ValuationAction(Variable variable, TruthValue *truthValue) {
  _logSyntacticAnalyzerAction(__FUNCTION__);
  Valuation *valuation = calloc(1, sizeof(Valuation));
  valuation->variable = variable;
  valuation->truthValue = truthValue;
  return valuation;
}

OpsetList *OpsetListAction(OpsetList *opsetList, Operator operator) {
  _logSyntacticAnalyzerAction(__FUNCTION__);
  OpsetList *newOpsetList = calloc(1, sizeof(OpsetList));
  newOpsetList->operator= operator;
  newOpsetList->next = opsetList;
  return newOpsetList;
}

EvaluateStatement *EvaluateFormulaAction(const char *formulaName,
                                         const char *valuationName) {
  _logSyntacticAnalyzerAction(__FUNCTION__);
  EvaluateStatement *evaluateStatement = calloc(1, sizeof(EvaluateStatement));
  evaluateStatement->formulaName = formulaName;
  evaluateStatement->valuationName = valuationName;
  return evaluateStatement;
}

AdequateStatement *CheckAdequacyAction(const char *opsetName) {
  _logSyntacticAnalyzerAction(__FUNCTION__);
  AdequateStatement *adequateStatement = calloc(1, sizeof(AdequateStatement));
  adequateStatement->opsetName = opsetName;
  return adequateStatement;
}

DefineVariable *DefineVariableAction(VariableList *variableList) {
  _logSyntacticAnalyzerAction(__FUNCTION__);
  DefineVariable *defineVariable = calloc(1, sizeof(DefineVariable));
  defineVariable->variableList = variableList;
  return defineVariable;
}

DefineFormula *DefineFormulaAction(const char *name, Expression *expression) {
  _logSyntacticAnalyzerAction(__FUNCTION__);
  DefineFormula *defineFormula = calloc(1, sizeof(DefineFormula));
  defineFormula->name = name;
  defineFormula->expression = expression;
  return defineFormula;
}

DefineValuation *DefineValuationAction(const char *name,
                                       ValuationList *valuationList) {
  _logSyntacticAnalyzerAction(__FUNCTION__);
  DefineValuation *defineValuation = calloc(1, sizeof(DefineValuation));
  defineValuation->name = name;
  defineValuation->valuationList = valuationList;
  return defineValuation;
}

DefineOperator *DefineOperatorAction(CustomOperator *customOperator,
                                     TruthTable *truthTable) {
  _logSyntacticAnalyzerAction(__FUNCTION__);
  DefineOperator *defineOperator = calloc(1, sizeof(DefineOperator));
  defineOperator->customOperator = customOperator;
  defineOperator->truthTable = truthTable;
  return defineOperator;
}

CustomOperator *DefineCustomOperatorAction(const char *name,
                                           VariableList *variableList) {
  _logSyntacticAnalyzerAction(__FUNCTION__);
  CustomOperator *customOperator = calloc(1, sizeof(CustomOperator));
  customOperator->name = name;
  customOperator->variableList = variableList;
  return customOperator;
}

DefineOpset *DefineOpsetAction(const char *name, OpsetList *opsetList) {
  _logSyntacticAnalyzerAction(__FUNCTION__);
  DefineOpset *defineOpset = calloc(1, sizeof(DefineOpset));
  defineOpset->name = name;
  defineOpset->opsetList = opsetList;
  return defineOpset;
}

Statement *
DefineVariableStatementSemanticAction(DefineVariable *defineVariable) {
  _logSyntacticAnalyzerAction(__FUNCTION__);
  Statement *statement = calloc(1, sizeof(Statement));
  statement->defineVariable = defineVariable;
  statement->type = DEFINE_VARIABLE;
  return statement;
}

Statement *DefineFormulaStatementSemanticAction(DefineFormula *defineFormula) {
  _logSyntacticAnalyzerAction(__FUNCTION__);
  Statement *statement = calloc(1, sizeof(Statement));
  statement->defineFormula = defineFormula;
  statement->type = DEFINE_FORMULA;
  return statement;
}

Statement *
DefineValuationStatementSemanticAction(DefineValuation *defineValuation) {
  _logSyntacticAnalyzerAction(__FUNCTION__);
  Statement *statement = calloc(1, sizeof(Statement));
  statement->defineValuation = defineValuation;
  statement->type = DEFINE_VALUATION;
  return statement;
}

Statement *
DefineOperatorStatementSemanticAction(DefineOperator *defineOperator) {
  _logSyntacticAnalyzerAction(__FUNCTION__);
  Statement *statement = calloc(1, sizeof(Statement));
  statement->defineOperator = defineOperator;
  statement->type = DEFINE_OPERATOR;
  return statement;
}

Statement *DefineOpsetStatementSemanticAction(DefineOpset *defineOpset) {
  _logSyntacticAnalyzerAction(__FUNCTION__);
  Statement *statement = calloc(1, sizeof(Statement));
  statement->defineOpset = defineOpset;
  statement->type = DEFINE_OPSET;
  return statement;
}

Statement *
EvaluateStatementSemanticAction(EvaluateStatement *evaluateStatement) {
  _logSyntacticAnalyzerAction(__FUNCTION__);
  Statement *statement = calloc(1, sizeof(Statement));
  statement->evaluateStatement = evaluateStatement;
  statement->type = EVALUATE_STATEMENT;
  return statement;
}

Statement *
AdequateStatementSemanticAction(AdequateStatement *adequateStatement) {
  _logSyntacticAnalyzerAction(__FUNCTION__);
  Statement *statement = calloc(1, sizeof(Statement));
  statement->adequateStatement = adequateStatement;
  statement->type = ADEQUATE_STATEMENT;
  return statement;
}

Program *ProgramStatementSemanticAction(CompilerState *compilerState,
                                        Program *nextProgram,
                                        Statement *statement) {
  _logSyntacticAnalyzerAction(__FUNCTION__);
  Program *program = calloc(1, sizeof(Program));
  program->statement = statement;
  program->next = nextProgram;

  compilerState->abstractSyntaxtTree = program;
  if (0 < flexCurrentContext()) {
    logError(_logger, "The final context is not the default (0): %d",
             flexCurrentContext());
    compilerState->succeed = false;
  } else {
    compilerState->succeed = true;
  }
  return program;
}