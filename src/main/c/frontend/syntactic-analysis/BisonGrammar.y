%{
#include "BisonActions.h"
#include "AbstractSyntaxTree.h"
%}

// You touch this, and you die.
%define api.value.union.name SemanticValue

%union {
    /** Terminals. */
    char truth_val;      // Para valores booleanos (TRUE o FALSE).
    Token token;          // Para tokens generales.

    /** Non-terminals. */
    Program *program;     // Representa el programa completo.
    Statement *statement; // Representa una declaración individual.
    Expression *expression; // Representa una expresión lógica o aritmética.
    ValuationList *valuationList; // Lista de valuaciones.
    ParameterList *parameterList; // Lista de parámetros para operadores.
    TruthTable *truthTable; // Tabla de verdad para operadores.
    TruthTableEntry *truthTableEntry; // Entrada individual de una tabla de verdad.
    TruthValues *truthValues; // Lista de valores de verdad en una entrada de tabla.
    TruthValue *truthValue; // Valor de verdad individual (TRUE o FALSE).
	TruthValueWildcard *truthValueWildcard; // Valor de verdad con comodín.
    OpsetList *opsetList; // Lista de conectivos en un conjunto de operadores.
    DefineVariable *defineVariable; // Declaración de variables.
    DefineFormula *defineFormula; // Declaración de fórmulas.
    DefineValuation *defineValuation; // Declaración de valuaciones.
    DefineOperator *defineOperator; // Declaración de operadores.
    DefineOpset *defineOpset; // Declaración de conjuntos de operadores.
    EvaluateStatement *evaluateStatement; // Evaluación de una fórmula.
    AdequateStatement *adequateStatement; // Verificación de adecuación.
}

/**
 * Destructors. This functions are executed after the parsing ends, so if the
 * AST must be used in the following phases of the compiler you shouldn't used
 * this approach. To use this mechanism, the AST must be translated into
 * another structure.
 *
 * @see https://www.gnu.org/software/bison/manual/html_node/Destructor-Decl.html
 */
/*
%destructor { releaseConstant($$); } <constant>
%destructor { releaseExpression($$); } <expression>
%destructor { releaseFactor($$); } <factor>
%destructor { releaseProgram($$); } <program>
*/

/** Terminals. */
%token <token> DEFINE VARIABLE FORMULA VALUATION OPERATOR OPSET EVALUATE ADEQUATE OTHERWISE
%token <token> AND OR THEN IFF NOT
%token <token> IDENTIFIER
%token <token> EQUALS OPEN_BRACE CLOSE_BRACE OPEN_PARENTHESIS CLOSE_PARENTHESIS COMMA SEMICOLON ARROW WILDCARD DOLLAR
%token <truth_val> TRUE
%token <truth_val> FALSE
%token <token> UNKNOWN

/** Non-terminals. */
%type <program> program
%type <expression> expression
%type <statement> statement
%type <valuationList> valuationList
%type <parameterList> parameterList
%type <truthTable> truthTable
%type <truthTableEntry> truthTableEntry
%type <truthValues> truthValues
%type <truthValue> truthValue
%type <truthValueWildcard> truthValueWildcard
%type <opsetList> opsetList
%type <defineVariable> defineVariable
%type <defineFormula> defineFormula
%type <defineValuation> defineValuation
%type <defineOperator> defineOperator
%type <defineOpset> defineOpset
%type <evaluateStatement> evaluateStatement
%type <adequateStatement> adequateStatement

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

program: program statement SEMICOLON								{ $$ = ProgramStatementSemanticAction($1, $2); }
	| statement SEMICOLON											{ $$ = SingleStatementProgramSemanticAction($1); }
	| %empty
	;

statement: defineVariable											{ $$ = DefineVariableStatementSemanticAction($1); }
	| defineFormula 												{ $$ = DefineFormulaStatementSemanticAction($1); }
	| defineValuation 												{ $$ = DefineValuationStatementSemanticAction($1); }
	| defineOperator 												{ $$ = DefineOperatorStatementSemanticAction($1); }
	| defineOpset 													{ $$ = DefineOpsetStatementSemanticAction($1); }
	| evaluateStatement 											{ $$ = EvaluateStatementSemanticAction($1); }
	| adequateStatement 											{ $$ = AdequateStatementSemanticAction($1); }
	;

defineVariable: DEFINE VARIABLE IDENTIFIER 							{ $$ = DefineVariableAction($3); }
	| DEFINE VARIABLE IDENTIFIER COMMA IDENTIFIER 					{ $$ = DefineMultipleVariablesAction($3, $5); }
	;

defineFormula: DEFINE FORMULA IDENTIFIER EQUALS expression 			{ $$ = DefineFormulaAction($3, $5); }
	;

defineValuation: DEFINE VALUATION IDENTIFIER EQUALS OPEN_BRACE valuationList CLOSE_BRACE 										{ $$ = DefineValuationAction($3, $6); }
	;

valuationList: IDENTIFIER EQUALS truthValue 						{ $$ = ValuationListSingleAction($1, $3); }
	| valuationList COMMA IDENTIFIER EQUALS truthValue 				{ $$ = ValuationListMultipleAction($1, $3, $5); }
	;

defineOperator: DEFINE OPERATOR IDENTIFIER OPEN_PARENTHESIS parameterList CLOSE_PARENTHESIS EQUALS expression 					{ $$ = DefineOperatorAction($3, $5, $8); }
	| DEFINE OPERATOR IDENTIFIER OPEN_PARENTHESIS parameterList CLOSE_PARENTHESIS EQUALS OPEN_BRACE truthTable CLOSE_BRACE 		{ $$ = DefineOperatorWithTruthTableAction($3, $5, $9); }
	;

parameterList: IDENTIFIER 											{ $$ = ParameterListSingleAction($1); }
	| parameterList COMMA IDENTIFIER 								{ $$ = ParameterListMultipleAction($1, $3); }
	;

truthTable: truthTableEntry 										{ $$ = TruthTableSingleAction($1); }
	| truthTable truthTableEntry 									{ $$ = TruthTableMultipleAction($1, $2); }
	;

truthTableEntry: OPEN_PARENTHESIS truthValues CLOSE_PARENTHESIS ARROW truthValue SEMICOLON										{ $$ = TruthTableEntryAction($2, $5); }
	| truthValue OTHERWISE SEMICOLON 								{ $$ = TruthTableEntryAction($1); }
	;

truthValues: truthValueWildcard 									{ $$ = TruthValuesSingleAction($1); }
	| truthValues COMMA truthValueWildcard 							{ $$ = TruthValuesMultipleAction($1, $3); }
	;

truthValueWildcard: WILDCARD 										{ $$ = WildcardSemanticAction(); }
    | truthValue 													{ $$ = TruthValueWildcardAction($1); }
    ;

truthValue: TRUE 													{ $$ = TruthValueAction(true); }
    | FALSE      													{ $$ = TruthValueAction(false); }
    ;

defineOpset: DEFINE OPSET IDENTIFIER EQUALS OPEN_BRACE opsetList CLOSE_BRACE 													{ $$ = DefineOpsetAction($3, $6); }
	;

opsetList: IDENTIFIER 												{ $$ = OpsetListSingleAction($1); }
	| opsetList COMMA IDENTIFIER 									{ $$ = OpsetListMultipleAction($1, $3); }
	;

evaluateStatement: EVALUATE OPEN_PARENTHESIS IDENTIFIER COMMA IDENTIFIER CLOSE_PARENTHESIS 										{ $$ = EvaluateFormulaAction($3, $5); }
	;

adequateStatement: ADEQUATE OPEN_PARENTHESIS IDENTIFIER CLOSE_PARENTHESIS														{ $$ = CheckAdequacyAction($3); }
	;

expression: expression[left] AND expression[right] 				    { $$ = LogicalExpressionSemanticAction($left, $right, AND); }
    | expression[left] OR expression[right]  						{ $$ = LogicalExpressionSemanticAction($left, $right, OR); }
    | expression[left] THEN expression[right] 						{ $$ = LogicalExpressionSemanticAction($left, $right, THEN); }
    | expression[left] IFF expression[right] 						{ $$ = LogicalExpressionSemanticAction($left, $right, IFF); }
    | NOT expression 												{ $$ = NotExpressionSemanticAction($2); }
    | OPEN_PARENTHESIS expression CLOSE_PARENTHESIS 				{ $$ = ParenthesizedExpressionSemanticAction($2); }
	| DOLLAR OPEN_BRACE IDENTIFIER CLOSE_BRACE 						{ $$ = PredefinedFormulaExpressionSemanticAction($3); }
    | IDENTIFIER			 										{ $$ = IdentifierExpressionSemanticAction($1); }
    ;

%%
