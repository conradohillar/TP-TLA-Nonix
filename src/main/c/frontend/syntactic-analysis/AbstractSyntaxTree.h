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
typedef char * Variable;

typedef struct DefineFormula DefineFormula;
typedef char * PredefinedFormula;

typedef struct DefineValuation DefineValuation;
typedef struct ValuationList ValuationList;
typedef struct Valuation Valuation;

typedef struct DefineOpset DefineOpset;
typedef struct OpsetList OpsetList;
typedef char * Operator;

typedef struct DefineOperator DefineOperator;
typedef struct BinaryOperator BinaryOperator;
typedef struct CustomOperator CustomOperator;

typedef struct AdequateStatement AdequateStatement;
typedef struct EvaluateStatement EvaluateStatement;

typedef struct TruthTable TruthTable;
typedef struct TruthTableEntryList TruthTableEntryList;
typedef struct TruthTableEntry TruthTableEntry;
typedef struct TruthValueList TruthValueList;
typedef struct TruthValueOrWildcard TruthValueOrWildcard;

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
	AND,
	OR,
	THEN,
	IFF
};

enum CustomExpressionType {
	PREDEFINED_FORMULA,
	CUSTOM_OPERATOR
};

enum TruthTableEntryType {
	TRUTH_VALUE_LIST,
	OTHERWISE
};

enum TruthValueOrWildcardType {
	TRUTH_VALUE,
	WILDCARD
};

// FORMULA
struct DefineFormula {
	char * name;
	Expression * expression;
};


// EXPRESSION
struct Expression {
	union {
		NotExpression * notExpression;
		BinaryExpression * binaryExpression;
		CustomExpression * customExpression;
		Variable * variable;
	};
	ExpressionType type;
};

struct NotExpression {
	Expression * expression;
};

struct BinaryExpression {
	Expression * leftExpression;
	Expression * rightExpression;
	BinaryOperatorType operator;
};

struct CustomExpression {
	union {
		PredefinedFormula * predefinedFormula;
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
	char * name;
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
	char * name;
	ValuationList * valuationList;
};

struct ValuationList {
	Valuation * valuation;
	ValuationList * next;
};

struct Valuation {
	Variable * variable;
	boolean value;
};

//OPSET
struct DefineOpset {
	char * name;
	OpsetList * opsetList;
};

struct OpsetList {
	Operator operator;
	OpsetList * next;
};


// ADEQUACY
struct AdequateStatement {
	char * opsetName;
};


// EVALUATION
struct EvaluateStatement {
	char * formulaName;
	char * valuationName;
};

//TRUTH TABLE
struct TruthTable {
	TruthTableEntryList * entryList;
};

struct TruthTableEntryList {
	TruthTableEntry * entry;
	TruthTableEntryList * next;
};

struct TruthTableEntry {
	union {
		struct {
			TruthValueList * truthValueList;
			boolean mapValue; // Boolean to which the truth values list is mapped.
		}; // TRUTH_VALUE_LIST
		boolean otherwiseValue; // OTHERWISE
	};
	TruthTableEntryType type;
};

struct TruthValueList {
	TruthValueOrWildcard truthValueOrWildcard;
	TruthValueList * next;
};

struct TruthValueOrWildcard {
	boolean truthValue;
	// We do not need to store the wildcard value as it has no inherent value. 
	// Its existance is defined by the TruthValueWithWildcardType identifier.
	TruthValueOrWildcardType type;
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
void releaseTruthTableEntryList(TruthTableEntryList * truthTableEntryList);
void releaseTruthValueList(TruthValueList * truthValueList);
void releaseTruthValueOrWildcard(TruthValueOrWildcard * truthValueOrWildcard);

#endif
