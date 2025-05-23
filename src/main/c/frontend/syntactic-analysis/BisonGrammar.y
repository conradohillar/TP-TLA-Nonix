%{
#include "BisonActions.h"
#include "AbstractSyntaxTree.h"
#include "../../shared/Type.h"
%}

// You touch this, and you die.
%define api.value.union.name SemanticValue

%union {
    /** Terminals. */
    boolean truth_value;     // Para valores booleanos (TRUE o FALSE).
    const char * keywordOrSymbol;       // Para tokens generales.

    /** Non-terminals. */
    Program *program;     // Representa el programa completo.
    Statement *statement; // Representa una declaración individual.
    Expression *expression; // Representa una expresión lógica o aritmética.
	BinaryExpression *binaryExpression; // Expresión binaria usando conectivo lógico (AND, OR, etc.).
	CustomExpression *customExpression; // Expresión personalizada.
	NotExpression *notExpression; // Expresión NOT.
	VariableList *variableList; // Lista de variables.
    ValuationList *valuationList; // Lista de valuaciones.
	Valuation *valuation; // Representa una valuación individual.
    TruthTable *truthTable; // Tabla de verdad para operadores.
    TruthTableEntry *truthTableEntry; // Entrada individual de una tabla de verdad.
    TruthValueList *truthValueList; // Lista de valores de verdad en una entrada de tabla.
    TruthValue *truthValue; // Valor de verdad individual (TRUE o FALSE).
	TruthValueOrWildcard *truthValueOrWildcard; // Valor de verdad con comodín.
    OpsetList *opsetList; // Lista de conectivos en un conjunto de operadores.
    DefineVariable *defineVariable; // Declaración de variables.
    DefineFormula *defineFormula; // Declaración de fórmulas.
    DefineValuation *defineValuation; // Declaración de valuaciones.
	CustomOperator *customOperator; // Estructura de un operador personalizado.
    DefineOperator *defineOperator; // Declaración de operadores.
    DefineOpset *defineOpset; // Declaración de conjuntos de operadores.
    EvaluateStatement *evaluateStatement; // Evaluación de una fórmula.
    AdequateStatement *adequateStatement; // Verificación de adecuación.
	//LexicalAnalyzerContext *lexicalAnalizerContext; // Contexto del analizador léxico.
}

/**
 * Destructors. This functions are executed after the parsing ends, so if the
 * AST must be used in the following phases of the compiler you shouldn't used
 * this approach. To use this mechanism, the AST must be translated into
 * another structure.
 *
 * @see https://www.gnu.org/software/bison/manual/html_node/Destructor-Decl.html
 */


%destructor { releaseProgram($$); } <program>
%destructor { releaseStatement($$); } <statement>

%destructor { releaseExpression($$); } <expression>
%destructor { releaseBinaryExpression($$); } <binaryExpression>
%destructor { releaseCustomExpression($$); } <customExpression>
%destructor { releaseNotExpression($$); } <notExpression>

%destructor { releaseVariableList($$); } <variableList>
%destructor { releaseDefineVariable($$); } <defineVariable>

%destructor { releaseDefineFormula($$); } <defineFormula>

%destructor { releaseDefineValuation($$); } <defineValuation>
%destructor { releaseValuationList($$); } <valuationList>
%destructor { releaseValuation($$); } <valuation>


%destructor { releaseOpsetList($$); } <opsetList>
%destructor { releaseDefineOpset($$); } <defineOpset>

%destructor { releaseDefineOperator($$); } <defineOperator>
%destructor { releaseCustomOperator($$); } <customOperator>

%destructor { releaseEvaluateStatement($$); } <evaluateStatement>
%destructor { releaseAdequateStatement($$); } <adequateStatement>


%destructor { releaseTruthTable($$); } <truthTable>
%destructor { releaseTruthTableEntry($$); } <truthTableEntry>
%destructor { releaseTruthValueList($$); } <truthValueList>
%destructor { releaseTruthValueOrWildcard($$); } <truthValueOrWildcard>
%destructor { releaseTruthValue($$); } <truthValue>

//%destructor { destroyLexicalAnalyzerContext($$); } <lexicalAnalizerContext>




/** Terminals. */
%token <keywordOrSymbol> DEFINE VARIABLE FORMULA VALUATION OPERATOR OPSET EVALUATE ADEQUATE OTHERWISE
%token <keywordOrSymbol> AND OR THEN IFF NOT
%token <keywordOrSymbol> IDENTIFIER
%token <keywordOrSymbol> EQUALS OPEN_BRACE CLOSE_BRACE OPEN_PARENTHESIS CLOSE_PARENTHESIS COMMA SEMICOLON ARROW WILDCARD DOLLAR
%token <truth_value> TRUE FALSE
%token <keywordOrSymbol> UNKNOWN

/** Non-terminals. */
%type <program> program
%type <expression> expression
%type <binaryExpression> binaryExpression
%type <customExpression> customExpression
%type <notExpression> notExpression
%type <statement> statement
%type <variableList> variableList
%type <valuationList> valuationList
%type <valuation> valuation
%type <truthTable> truthTable
%type <truthTableEntry> truthTableEntry
%type <truthValueList> truthValueList
%type <truthValue> truthValue
%type <truthValueOrWildcard> truthValueOrWildcard
%type <opsetList> opsetList
%type <defineVariable> defineVariable
%type <defineFormula> defineFormula
%type <defineValuation> defineValuation
%type <defineOperator> defineOperator
%type <defineOpset> defineOpset
%type <evaluateStatement> evaluateStatement
%type <adequateStatement> adequateStatement
%type <customOperator> customOperator

/**
 * Precedence and associativity.
 *
 * @see https://www.gnu.org/software/bison/manual/html_node/Precedence.html
 */
/*
%left ADD SUB
%left MUL DIV
*/
%%

// IMPORTANT: To use λ in the following grammar, use the %empty symbol.

program: program statement SEMICOLON																{ $$ = ProgramStatementSemanticAction(currentCompilerState(), $1, $2); }
	| statement SEMICOLON																			{ $$ = ProgramStatementSemanticAction(currentCompilerState(), NULL, $1); }
	;

statement: defineVariable																			{ $$ = DefineVariableStatementSemanticAction($1); }
	| defineFormula 																				{ $$ = DefineFormulaStatementSemanticAction($1); }
	| defineValuation 																				{ $$ = DefineValuationStatementSemanticAction($1); }
	| defineOperator 																				{ $$ = DefineOperatorStatementSemanticAction($1); }
	| defineOpset 																					{ $$ = DefineOpsetStatementSemanticAction($1); }
	| evaluateStatement 																			{ $$ = EvaluateStatementSemanticAction($1); }
	| adequateStatement 																			{ $$ = AdequateStatementSemanticAction($1); }
	;

defineVariable: DEFINE VARIABLE variableList			    										{ $$ = DefineVariableAction($3); }
	;

defineFormula: DEFINE FORMULA IDENTIFIER EQUALS expression 											{ $$ = DefineFormulaAction($3, $5); }
	;

defineValuation: DEFINE VALUATION IDENTIFIER EQUALS OPEN_BRACE valuationList CLOSE_BRACE 			{ $$ = DefineValuationAction($3, $6); }
	;

variableList: IDENTIFIER 																			{ $$ = VariableListAction(NULL, $1); }
	| variableList COMMA IDENTIFIER 																{ $$ = VariableListAction($1, $3); }
	;

valuationList: valuation 																			{ $$ = ValuationListAction(NULL, $1); }
	| valuationList COMMA valuation 																{ $$ = ValuationListAction($1, $3); }
	;

valuation: IDENTIFIER EQUALS truthValue																{ $$ = ValuationAction($1, $3); }
	;

defineOperator: DEFINE OPERATOR customOperator EQUALS OPEN_BRACE truthTable CLOSE_BRACE 			{ $$ = DefineOperatorAction($3, $6); }
	;

customOperator: IDENTIFIER OPEN_PARENTHESIS variableList CLOSE_PARENTHESIS     						{ $$ = DefineCustomOperatorAction($1, $3); }
	;

truthTable: truthTableEntry 																		{ $$ = TruthTableAction(NULL, $1); }
	| truthTable truthTableEntry 																	{ $$ = TruthTableAction($1, $2); }
	;

truthTableEntry: OPEN_PARENTHESIS truthValueList CLOSE_PARENTHESIS ARROW truthValue SEMICOLON		{ $$ = TruthTableMapperEntryAction($2, $5); }
	| truthValue OTHERWISE SEMICOLON 																{ $$ = TruthTableOtherwiseEntryAction($1); }
	;

truthValueList: truthValueOrWildcard 																{ $$ = TruthValueListAction(NULL, $1); }
	| truthValueList COMMA truthValueOrWildcard 													{ $$ = TruthValueListAction($1, $3); }
	;

truthValueOrWildcard: WILDCARD 																		{ $$ = WildcardTypeAction(); }
    | truthValue 																					{ $$ = TruthValueTypeAction($1); }
    ;

truthValue: TRUE 																					{ $$ = TruthValueAction($1); }
    | FALSE      																					{ $$ = TruthValueAction($1); }
    ;

defineOpset: DEFINE OPSET IDENTIFIER EQUALS OPEN_BRACE opsetList CLOSE_BRACE 						{ $$ = DefineOpsetAction($3, $6); }
	;

opsetList: IDENTIFIER 																				{ $$ = OpsetListAction(NULL, $1); }
	| opsetList COMMA IDENTIFIER 																	{ $$ = OpsetListAction($1, $3); }
	| opsetList COMMA AND 																			{ $$ = OpsetListAction($1, $3); }
	| opsetList COMMA OR 																			{ $$ = OpsetListAction($1, $3); }
	| opsetList COMMA THEN 																			{ $$ = OpsetListAction($1, $3); }
	| opsetList COMMA IFF 																			{ $$ = OpsetListAction($1, $3); }
	| opsetList COMMA NOT 																			{ $$ = OpsetListAction($1, $3); }
	| AND 																							{ $$ = OpsetListAction(NULL, $1); }
	| OR																							{ $$ = OpsetListAction(NULL, $1); }
	| THEN																							{ $$ = OpsetListAction(NULL, $1); }
	| IFF																							{ $$ = OpsetListAction(NULL, $1); }
	| NOT																							{ $$ = OpsetListAction(NULL, $1); }
	;

evaluateStatement: EVALUATE OPEN_PARENTHESIS IDENTIFIER COMMA IDENTIFIER CLOSE_PARENTHESIS			{ $$ = EvaluateFormulaAction($3, $5); }
	;

adequateStatement: ADEQUATE OPEN_PARENTHESIS IDENTIFIER CLOSE_PARENTHESIS							{ $$ = CheckAdequacyAction($3); }
	;

expression: binaryExpression																		{ $$ = BinaryTypeAction($1); }
	| customExpression																				{ $$ = CustomTypeAction($1); }
    | notExpression 																				{ $$ = NotTypeAction($1); }
    | IDENTIFIER																					{ $$ = VariableTypeAction($1); }
    ;	

binaryExpression: OPEN_PARENTHESIS expression[left] AND expression[right] CLOSE_PARENTHESIS			{ $$ = BinaryExpressionSemanticAction($left, $right, BINOP_AND); }
	| OPEN_PARENTHESIS expression[left] OR expression[right] CLOSE_PARENTHESIS						{ $$ = BinaryExpressionSemanticAction($left, $right, BINOP_OR); }
	| OPEN_PARENTHESIS expression[left] THEN expression[right] CLOSE_PARENTHESIS					{ $$ = BinaryExpressionSemanticAction($left, $right, BINOP_THEN); }
	| OPEN_PARENTHESIS expression[left] IFF expression[right] CLOSE_PARENTHESIS						{ $$ = BinaryExpressionSemanticAction($left, $right, BINOP_IFF); }
    ;

customExpression: DOLLAR OPEN_BRACE IDENTIFIER CLOSE_BRACE 											{ $$ = PredefinedFormulaSemanticAction($3); }
	| customOperator																				{ $$ = CustomOperatorSemanticAction($1); }	 																		
	;

notExpression: NOT expression 																		{ $$ = NotExpressionSemanticAction($2); }
	;

%%