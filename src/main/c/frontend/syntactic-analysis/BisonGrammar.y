%{
#include "BisonActions.h"
#include "AbstractSyntaxTree.h"
%}

// You touch this, and you die.
%define api.value.union.name SemanticValue

%union {
    /** Terminals. */
    int integer;          // Para valores enteros como TRUE (1) y FALSE (0).
    Token token;          // Para tokens generales.

    /** Non-terminals. */
    Program *program;     // Representa el programa completo.
    Statement *statement; // Representa una declaración individual.
    Expression *expression; // Representa una expresión lógica o aritmética.
    ValuationList *valuation_list; // Lista de valuaciones.
    ParameterList *parameter_list; // Lista de parámetros para operadores.
    TruthTable *truth_table; // Tabla de verdad para operadores.
    TruthTableEntry *truth_table_entry; // Entrada individual de una tabla de verdad.
    TruthValues *truth_values; // Lista de valores de verdad en una entrada de tabla.
    TruthValue *truth_value; // Valor de verdad individual (TRUE o FALSE).
	TruthValueWildcard *truth_value_wildcard; // Valor de verdad con comodín.
    OpsetList *opset_list; // Lista de conectivos en un conjunto de operadores.
    DefineVariable *define_variable; // Declaración de variables.
    DefineFormula *define_formula; // Declaración de fórmulas.
    DefineValuation *define_valuation; // Declaración de valuaciones.
    DefineOperator *define_operator; // Declaración de operadores.
    DefineOpset *define_opset; // Declaración de conjuntos de operadores.
    EvaluateStatement *evaluate_statement; // Evaluación de una fórmula.
    AdequateStatement *adequate_statement; // Verificación de adecuación.
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
%token <token> AND "&"
%token <token> OR "|"
%token <token> NOT "!"
%token <token> THEN "=>"
%token <token> IFF "<=>"
%token <token> IDENTIFIER
%token <token> EQUALS OPEN_BRACE CLOSE_BRACE OPEN_PARENTHESIS CLOSE_PARENTHESIS COMMA SEMICOLON ARROW WILDCARD DOLLAR
%token <token> UNKNOWN
%token <integer> TRUE "true"
%token <integer> FALSE "false"

/** Non-terminals. */
%type <program> program
%type <expression> expression
%type <statement> statement
%type <valuation_list> valuation_list
%type <parameter_list> parameter_list
%type <truth_table> truth_table
%type <truth_table_entry> truth_table_entry
%type <truth_values> truth_values
%type <truth_value> truth_value
%type <truth_value_wildcard> truth_value_wildcard
%type <opset_list> opset_list
%type <define_variable> define_variable
%type <define_formula> define_formula
%type <define_valuation> define_valuation
%type <define_operator> define_operator
%type <define_opset> define_opset
%type <evaluate_statement> evaluate_statement
%type <adequate_statement> adequate_statement

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
	;

statement: define_variable											{ $$ = DefineVariableStatementSemanticAction($1); }
	| define_formula 												{ $$ = DefineFormulaStatementSemanticAction($1); }
	| define_valuation 												{ $$ = DefineValuationStatementSemanticAction($1); }
	| define_operator 												{ $$ = DefineOperatorStatementSemanticAction($1); }
	| define_opset 													{ $$ = DefineOpsetStatementSemanticAction($1); }
	| evaluate_statement 											{ $$ = EvaluateStatementSemanticAction($1); }
	| adequate_statement 											{ $$ = AdequateStatementSemanticAction($1); }
	;

define_variable: DEFINE VARIABLE IDENTIFIER 						{ $$ = DefineVariableAction($3); }
	| DEFINE VARIABLE IDENTIFIER COMMA IDENTIFIER 					{ $$ = DefineMultipleVariablesAction($3, $5); }
	;

define_formula: DEFINE FORMULA IDENTIFIER EQUALS expression 		{ $$ = DefineFormulaAction($3, $5); }
	;

define_valuation: DEFINE VALUATION IDENTIFIER EQUALS OPEN_BRACE valuation_list CLOSE_BRACE 										{ $$ = DefineValuationAction($3, $6); }
	;

valuation_list: IDENTIFIER EQUALS truth_value 						{ $$ = ValuationListSingleAction($1, $3); }
	| valuation_list COMMA IDENTIFIER EQUALS truth_value 			{ $$ = ValuationListMultipleAction($1, $3, $5); }
	;

define_operator: DEFINE OPERATOR IDENTIFIER OPEN_PARENTHESIS parameter_list CLOSE_PARENTHESIS EQUALS expression 				{ $$ = DefineOperatorAction($3, $5, $8); }
	| DEFINE OPERATOR IDENTIFIER OPEN_PARENTHESIS parameter_list CLOSE_PARENTHESIS EQUALS OPEN_BRACE truth_table CLOSE_BRACE 	{ $$ = DefineOperatorWithTruthTableAction($3, $5, $9); }
	;

parameter_list: IDENTIFIER 											{ $$ = ParameterListSingleAction($1); }
	| parameter_list COMMA IDENTIFIER 								{ $$ = ParameterListMultipleAction($1, $3); }
	;

truth_table: truth_table_entry 										{ $$ = TruthTableSingleAction($1); }
	| truth_table truth_table_entry 								{ $$ = TruthTableMultipleAction($1, $2); }
	;

truth_table_entry: OPEN_PARENTHESIS truth_values CLOSE_PARENTHESIS ARROW truth_value SEMICOLON									{ $$ = TruthTableEntryAction($2, $5); }
	| truth_value OTHERWISE SEMICOLON 										{ $$ = TruthTableEntryAction($2, $5); }
	;

truth_values: truth_value_wildcard 									{ $$ = TruthValuesSingleAction($1); }
	| truth_values COMMA truth_value_wildcard 						{ $$ = TruthValuesMultipleAction($1, $3); }
	;

truth_value_wildcard: WILDCARD
	| truth_value 													{ $$ = TruthValueWildcardAction($1); }
	;

truth_value: TRUE 													{ $$ = TruthValueAction(true); }
    | FALSE      													{ $$ = TruthValueAction(false); }
    ;

define_opset: DEFINE OPSET IDENTIFIER EQUALS OPEN_BRACE opset_list CLOSE_BRACE 													{ $$ = DefineOpsetAction($3, $6); }
	;

opset_list: IDENTIFIER 												{ $$ = OpsetListSingleAction($1); }
	| opset_list COMMA IDENTIFIER 									{ $$ = OpsetListMultipleAction($1, $3); }
	;

evaluate_statement: EVALUATE OPEN_PARENTHESIS IDENTIFIER COMMA IDENTIFIER CLOSE_PARENTHESIS 									{ $$ = EvaluateFormulaAction($3, $5); }
	;

adequate_statement: ADEQUATE OPEN_PARENTHESIS IDENTIFIER CLOSE_PARENTHESIS														{ $$ = CheckAdequacyAction($3); }
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
