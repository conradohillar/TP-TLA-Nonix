#ifndef ABSTRACT_SYNTAX_TREE_HEADER
#define ABSTRACT_SYNTAX_TREE_HEADER

#include "../../shared/Logger.h"
#include "../../shared/Type.h"
#include <stdlib.h>

/** Initialize module's internal state. */
void initializeAbstractSyntaxTreeModule();

/** Shutdown module's internal state. */
void shutdownAbstractSyntaxTreeModule();

/**
 * This typedefs allows self-referencing types.
 */

typedef enum StatementType StatementType;
typedef enum ExpressionType ExpressionType;
typedef enum BinaryOperatorType BinaryOperatorType;
typedef enum CustomExpressionType CustomExpressionType;
typedef enum TruthTableEntryType TruthTableEntryType;
typedef enum TruthValueOrWildcardType TruthValueOrWildcardType;

typedef struct Program Program;
typedef struct Statement Statement;

typedef struct Expression Expression;
typedef struct NotExpression NotExpression;
typedef struct BinaryExpression BinaryExpression;
typedef struct CustomExpression CustomExpression;

typedef struct DefineVariable DefineVariable;
typedef struct VariableList VariableList;
typedef const char * Variable;

typedef struct DefineFormula DefineFormula;
typedef const char * PredefinedFormula;

typedef struct DefineValuation DefineValuation;
typedef struct ValuationList ValuationList;
typedef struct Valuation Valuation;

typedef struct DefineOpset DefineOpset;
typedef struct OpsetList OpsetList;

typedef struct DefineOperator DefineOperator;
typedef struct BinaryOperator BinaryOperator;
typedef struct CustomOperator CustomOperator;
typedef const char * Operator;

typedef struct AdequateStatement AdequateStatement;
typedef struct EvaluateStatement EvaluateStatement;

typedef struct TruthTable TruthTable;
typedef struct TruthTableEntry TruthTableEntry;
typedef struct TruthValueList TruthValueList;
typedef struct TruthValueOrWildcard TruthValueOrWildcard;
typedef struct TruthValue TruthValue;

/**
 * Node types for the Abstract Syntax Tree (AST).
 */

 enum ExpressionType {
	NOT_EXPRESSION,
	BINARY_EXPRESSION,
	CUSTOM_EXPRESSION,
	VARIABLE_EXPRESSION
};

enum BinaryOperatorType {
	BINOP_AND,
	BINOP_OR,
	BINOP_THEN,
	BINOP_IFF
};

enum CustomExpressionType {
	PREDEFINED_FORMULA,
	CUSTOM_OPERATOR
};

enum TruthTableEntryType {
	TRUTH_VALUE_LIST,
	OTHERWISE_ENTRY
};

enum TruthValueOrWildcardType {
	TRUTH_VALUE,
	WILDCARD_VALUE
};

// FORMULA
struct DefineFormula {
	const char * name;
	Expression * expression;
};


// EXPRESSION
struct Expression {
	union {
		NotExpression * notExpression;
		BinaryExpression * binaryExpression;
		CustomExpression * customExpression;
		Variable variable;
	};
	ExpressionType type;
};

struct NotExpression {
	Expression * expression;
};

struct BinaryExpression {
	Expression * leftExpression;
	Expression * rightExpression;
	BinaryOperatorType operatorType;
};

struct CustomExpression {
	union {
		PredefinedFormula predefinedFormula;
		CustomOperator * customOperator;
	};
	CustomExpressionType type;
};


// CUSTOM OPERATOR
struct DefineOperator {
	CustomOperator * customOperator;
	TruthTable * truthTable;
};

struct CustomOperator {
	const char * name;
	VariableList * variableList;
};

// VARIABLE
struct DefineVariable {
	VariableList * variableList;
};

struct VariableList {
	Variable variable;
	VariableList * next;
};

// VALUATION
struct DefineValuation {
	const char * name;
	ValuationList * valuationList;
};

struct ValuationList {
	Valuation * valuation;
	ValuationList * next;
};

struct Valuation {
	Variable variable;
	TruthValue * truthValue;
};

//OPSET
struct DefineOpset {
	const char * name;
	OpsetList * opsetList;
};

struct OpsetList {
	Operator operator;
	OpsetList * next;
};


// ADEQUACY
struct AdequateStatement {
	const char * opsetName;
};


// EVALUATION
struct EvaluateStatement {
	const char * formulaName;
	const char * valuationName;
};

//TRUTH TABLE
struct TruthTable {
	TruthTableEntry * entry;
	TruthTable * next;
};

struct TruthTableEntry {
	union {
		struct {
			TruthValueList * truthValueList;
			TruthValue * mapValue; // Boolean to which the truth values list is mapped.
		}; // TRUTH_VALUE_LIST
		TruthValue * otherwiseValue; // OTHERWISE
	};
	TruthTableEntryType type;
};

struct TruthValueList {
	TruthValueOrWildcard * truthValueOrWildcard;
	TruthValueList * next;
};

struct TruthValueOrWildcard {
	TruthValue * truthValue;
	// We do not need to store the wildcard value as it has no inherent value. 
	// Its existance is defined by the TruthValueWithWildcardType identifier.
	TruthValueOrWildcardType type;
};

struct TruthValue {
	boolean value;
};


// STATEMENT
enum StatementType {
	DEFINE_VARIABLE,
	DEFINE_FORMULA,
	DEFINE_VALUATION,
	DEFINE_OPERATOR,
	DEFINE_OPSET,
	EVALUATE_STATEMENT,
	ADEQUATE_STATEMENT
};

struct Statement {
	union {
		DefineVariable * defineVariable;
		DefineFormula * defineFormula;
		DefineValuation * defineValuation;
		DefineOperator * defineOperator;
		DefineOpset * defineOpset;
		EvaluateStatement * evaluateStatement;
		AdequateStatement * adequateStatement;
	};
	StatementType type;
};

struct Program {
	Statement * statement;
	Program * next;	
};

/**
 * Node recursive destructors.
 */
void releaseProgram(Program * program);
void releaseStatement(Statement * statement);

void releaseExpression(Expression * expression);
void releaseNotExpression(NotExpression * notExpression);
void releaseBinaryExpression(BinaryExpression * binaryExpression);
void releaseCustomExpression(CustomExpression * customExpression);

void releaseDefineVariable(DefineVariable * defineVariable);
void releaseVariableList(VariableList * variableList);

void releaseDefineFormula(DefineFormula * defineFormula);

void releaseDefineValuation(DefineValuation * defineValuation);
void releaseValuationList(ValuationList * valuationList);
void releaseValuation(Valuation * valuation);

void releaseDefineOpset(DefineOpset * defineOpset);
void releaseOpsetList(OpsetList * opsetList);

void releaseDefineOperator(DefineOperator * defineOperator);
void releaseCustomOperator(CustomOperator * customOperator);

void releaseAdequateStatement(AdequateStatement * adequateStatement);
void releaseEvaluateStatement(EvaluateStatement * evaluateStatement);

void releaseTruthTable(TruthTable * truthTable);
void releaseTruthTableEntry(TruthTableEntry * truthTableEntry);
void releaseTruthValueList(TruthValueList * truthValueList);
void releaseTruthValueOrWildcard(TruthValueOrWildcard * truthValueOrWildcard);
void releaseTruthValue(TruthValue * truthValue);

#endif
