#include "Generator.h"

/* MODULE INTERNAL STATE */

const char _indentationCharacter = ' ';
const char _indentationSize = 4;
static Logger * _logger = NULL;
static FILE * _outputFile = NULL;

void initializeGeneratorModule() {
	_logger = createLogger("Generator");
	_outputFile = fopen("output.tex", "w");
	if (_outputFile == NULL) {
		logError(_logger, "Cannot open output file");
		exit(1);
	}
}

void shutdownGeneratorModule() {
	if (_outputFile != NULL) {
		fclose(_outputFile);
	}
	if (_logger != NULL) {
		destroyLogger(_logger);
	}
}


/** PRIVATE FUNCTIONS */
static void _generateProgram(Program * program);
static void _generateStatement(const unsigned int indentationLevel, Statement * statement);
static void _generateVariableDefinition(const unsigned int indentationLevel, DefineVariable * defineVariable);
static void _generateFormulaDefinition(const unsigned int indentationLevel, DefineFormula * defineFormula);
static void _generateValuationDefinition(const unsigned int indentationLevel, DefineValuation * defineValuation);
static void _generateAdequateStatement(const unsigned int indentationLevel, AdequateStatement * adequateStatement);
static void _generateEvaluateStatement(const unsigned int indentationLevel, EvaluateStatement * evaluateStatement);
static void _generateOpsetDefinition(const unsigned int indentationLevel, DefineOpset * defineOpset);
static void _generateOperatorDefinition(const unsigned int indentationLevel, DefineOperator * defineOperator);
static void _generateExpression(const unsigned int indentationLevel, Expression * expression);
static void _generateCustomExpression(const unsigned int indentationLevel, CustomExpression * customExpression);
static void _generateBinaryExpression(const unsigned int indentationLevel, BinaryExpression * binaryExpression);
static void _generateTruthValue(const unsigned int indentationLevel, TruthValue * truthValue);
static void _generateValuationList(const unsigned int indentationLevel, ValuationList * valuationList);
static void _generateVariableList(const unsigned int indentationLevel, VariableList * variableList);
static void _generateOpsetList(const unsigned int indentationLevel, OpsetList * opsetList);
static void _generateTruthTable(const unsigned int indentationLevel, TruthTable * truthTable);
static void _generateTruthTableEntry(const unsigned int indentationLevel, TruthTableEntry * entry);
static void _generateTruthValueList(const unsigned int indentationLevel, TruthValueList * truthValueList);
static void _generateTruthValueOrWildcard(const unsigned int indentationLevel, TruthValueOrWildcard * truthValueOrWildcard);
static void _generateResults(ComputedValue * values);

static char * _indentation(const unsigned int indentationLevel);
static void _output(const unsigned int indentationLevel, const char * const format, ...);


static void _generateAdequateStatement(const unsigned int indentationLevel, AdequateStatement * adequateStatement) {
	_output(indentationLevel, "%s", "adequate(");
	_output(indentationLevel, "%s", adequateStatement->opsetName);
	_output(indentationLevel, "%s", ");\n");
}
static void _generateEvaluateStatement(const unsigned int indentationLevel, EvaluateStatement * evaluateStatement) {
	_output(indentationLevel, "%s", "evaluate(");
	_output(indentationLevel, "%s", evaluateStatement->formulaName);
	_output(indentationLevel, "%s", ", ");
	_output(indentationLevel, "%s", evaluateStatement->valuationName);
	_output(indentationLevel, "%s", ");\n");
}

static void _generateOpsetList(const unsigned int indentationLevel, OpsetList * opsetList) {
	if (opsetList->next != NULL) {
		_generateOpsetList(indentationLevel, opsetList->next);
		_output(indentationLevel, "%s", ", ");
	}
	_output(indentationLevel, "%s", opsetList->operator);
}

static void _generateOpsetDefinition(const unsigned int indentationLevel, DefineOpset * defineOpset) {
	_output(indentationLevel, "%s", "define opset ");
	_output(indentationLevel, "%s", defineOpset->name);
	_output(indentationLevel, "%s", " = {");
	_generateOpsetList(indentationLevel, defineOpset->opsetList);
	_output(indentationLevel, "%s", "};\n");
}

static void _generateTruthTable(const unsigned int indentationLevel, TruthTable * truthTable) {
	if (truthTable == NULL) {
		return;
	}
	_generateTruthTable(indentationLevel, truthTable->next);
	_generateTruthTableEntry(indentationLevel, truthTable->entry);
}

static void _generateTruthTableEntry(const unsigned int indentationLevel, TruthTableEntry * entry) {
	switch (entry->type) {
		case TRUTH_VALUE_LIST:
			_output(indentationLevel, "%s", "  (");
			_generateTruthValueList(indentationLevel, entry->truthValueList);
			_output(indentationLevel, "%s", ") -> ");
			_generateTruthValue(indentationLevel, entry->mapValue);
			_output(indentationLevel, "%s", ";\n");
			break;
		case OTHERWISE_ENTRY:
			_output(indentationLevel, "%s", "  ");
			_generateTruthValue(indentationLevel, entry->otherwiseValue);
			_output(indentationLevel, "%s", " otherwise;\n");
			break;
		default:
			logError(_logger, "Unknown truth table entry type: %d", entry->type);
			break;
	}
}

static void _generateTruthValueList(const unsigned int indentationLevel, TruthValueList * truthValueList) {
	if (truthValueList->next != NULL) {
		_generateTruthValueList(indentationLevel, truthValueList->next);
		_output(indentationLevel, "%s", ", ");
	}
	_generateTruthValueOrWildcard(indentationLevel, truthValueList->truthValueOrWildcard);
}

static void _generateTruthValueOrWildcard(const unsigned int indentationLevel, TruthValueOrWildcard * truthValueOrWildcard) {
	switch (truthValueOrWildcard->type) {
		case TRUTH_VALUE:
			_generateTruthValue(indentationLevel, truthValueOrWildcard->truthValue);
			break;
		case WILDCARD_VALUE:
			_output(indentationLevel, "%s", "?");
			break;
		default:
			logError(_logger, "Unknown truth value or wildcard type: %d", truthValueOrWildcard->type);
			break;
	}
}

static void _generateTruthValue(const unsigned int indentationLevel, TruthValue * truthValue) {
	const char * char_value;
	if (truthValue->value) {
		char_value = "true";
	} else {
		char_value = "false";
	}
	_output(indentationLevel, "%s", char_value);
}

static void _generateOperatorDefinition(const unsigned int indentationLevel, DefineOperator * defineOperator) {
	_output(indentationLevel, "%s", "define operator ");
	_output(indentationLevel, "%s(", defineOperator->customOperator->name);
	_generateVariableList(indentationLevel, defineOperator->customOperator->variableList);
	_output(indentationLevel, "%s", ") = {\n");
	_generateTruthTable(indentationLevel, defineOperator->truthTable);
	_output(indentationLevel, "%s", "};\n");
}

static void _generateCustomExpression(const unsigned int indentationLevel, CustomExpression * customExpression) {
	switch (customExpression->type) {
		case PREDEFINED_FORMULA:
			_output(indentationLevel, "%s", "${");
			_output(indentationLevel, "%s", customExpression->predefinedFormula);
			_output(indentationLevel, "%s", "}");
			break;
		case CUSTOM_OPERATOR:
			_output(indentationLevel, "%s(", customExpression->customOperator->name);
			_generateVariableList(indentationLevel, customExpression->customOperator->variableList);
			_output(indentationLevel, "%s", ")");
			break;
		default:
			logError(_logger, "Unknown custom expression type: %d", customExpression->type);
			break;
	}
}

static void _generateBinaryExpression(const unsigned int indentationLevel, BinaryExpression * binaryExpression) {
	_output(indentationLevel, "(");
	_generateExpression(indentationLevel, binaryExpression->leftExpression);
	switch(binaryExpression->operatorType) {
		case BINOP_AND:
			_output(indentationLevel, " %s ", "&");
			break;
		case BINOP_OR:
			_output(indentationLevel, " %s ", "|");
			break;
		case BINOP_THEN:
			_output(indentationLevel, " %s ", "=>");
			break;
		case BINOP_IFF:
			_output(indentationLevel, " %s ", "<=>");
			break;
		default:
			logError(_logger, "Unknown binary operator type: %d", binaryExpression->operatorType);
			break;
	}
	_generateExpression(indentationLevel, binaryExpression->rightExpression);
	_output(indentationLevel, ")");
}

static void _generateExpression(const unsigned int indentationLevel, Expression * expression) {
	switch (expression->type) {
		case NOT_EXPRESSION:
			_output(indentationLevel, "%s", "!");
			_generateExpression(indentationLevel, expression->notExpression->expression);
			break;
		case BINARY_EXPRESSION:
			_generateBinaryExpression(indentationLevel, expression->binaryExpression);
			break;
		case CUSTOM_EXPRESSION:
			_generateCustomExpression(indentationLevel, expression->customExpression);
			break;
		case VARIABLE_EXPRESSION:
			_output(indentationLevel, "%s", expression->variable);
			break;
		default:
			logError(_logger, "Unknown expression type: %d", expression->type);
			break;
	}
}

static void _generateValuationList(const unsigned int indentationLevel, ValuationList * valuationList) {
	if(valuationList->next != NULL) {
		_generateValuationList(indentationLevel, valuationList->next);
		_output(indentationLevel, "%s", ", ");
	}
	_output(indentationLevel, "%s", valuationList->valuation->variable);
	_output(indentationLevel, "%s", " = ");
	_generateTruthValue(indentationLevel, valuationList->valuation->truthValue);
}

static void _generateValuationDefinition(const unsigned int indentationLevel, DefineValuation * defineValuation) {
	_output(indentationLevel, "%s", "define valuation ");
	_output(indentationLevel, "%s", defineValuation->name);
	_output(indentationLevel, "%s", " = {");
	_generateValuationList(indentationLevel, defineValuation->valuationList);
	_output(indentationLevel, "%s", "};\n");
}

static void _generateFormulaDefinition(const unsigned int indentationLevel, DefineFormula * defineFormula) {
	_output(indentationLevel, "%s", "define formula ");
	_output(indentationLevel, "%s", defineFormula->name);
	_output(indentationLevel, "%s", " = ");
	_generateExpression(indentationLevel, defineFormula->expression);
	_output(indentationLevel, "%s", ";\n");
}

static void _generateVariableList(const unsigned int indentationLevel, VariableList * variableList) {
	if(variableList->next != NULL) {
		_generateVariableList(indentationLevel, variableList->next);
		_output(indentationLevel, "%s", ", ");
	}
	_output(indentationLevel, "%s", variableList->variable);
}

static void _generateVariableDefinition(const unsigned int indentationLevel, DefineVariable * defineVariable) {
	_output(indentationLevel, "%s", "define variable ");
	_generateVariableList(indentationLevel, defineVariable->variableList);
	_output(indentationLevel, "%s", ";\n");
}

/**
 * Generates the output of a statement.
 */
static void _generateStatement(const unsigned int indentationLevel, Statement * statement) {
	switch (statement->type) {
		case DEFINE_VARIABLE:
			_generateVariableDefinition(indentationLevel, statement->defineVariable);
			break;
		case DEFINE_FORMULA:
			_generateFormulaDefinition(indentationLevel, statement->defineFormula);
			break;
		case DEFINE_VALUATION:
			_generateValuationDefinition(indentationLevel, statement->defineValuation);
			break;
		case DEFINE_OPERATOR:
			_generateOperatorDefinition(indentationLevel, statement->defineOperator);
			break;
		case DEFINE_OPSET:
			_generateOpsetDefinition(indentationLevel, statement->defineOpset);
			break;
		case EVALUATE_STATEMENT:
			_generateEvaluateStatement(indentationLevel, statement->evaluateStatement);
			break;
		case ADEQUATE_STATEMENT:
			_generateAdequateStatement(indentationLevel, statement->adequateStatement);
			break;
		default:
			logError(_logger, "The specified statement type is unknown: %d", statement->type);
			break;
	}
}

/**
 * Generates the output of the program.
 */
static void _generateProgram(Program * program) {
	if(program == NULL) {
		return;
	}
	_generateProgram(program->next);
	_generateStatement(0, program->statement);
}

/**
 * Creates the prologue of the generated output, a Latex document that renders
 * a tree thanks to the Forest package.
 *
 * @see https://ctan.dcc.uchile.cl/graphics/pgf/contrib/forest/forest-doc.pdf
 */
static void _generatePrologue(void) {
	_output(0, "%s",
    "\\documentclass{article}\n"
    "\\usepackage[utf8]{inputenc}\n"
    "\\usepackage[T1]{fontenc}\n"
    "\\usepackage{amsmath}\n"
    "\\usepackage{microtype}\n"
    "\\usepackage{listings}\n"
    "\\usepackage{xcolor}\n"
    "\\usepackage{tcolorbox}\n"
    "\\date{}\n"
    "\\definecolor{codegray}{gray}{0.95}\n"
    "\\lstdefinestyle{mystyle}{backgroundcolor=\\color{codegray},basicstyle=\\ttfamily\\small,frame=single,columns=fullflexible,keepspaces=true}\n"
    "\\begin{document}\n"
    "\\section*{Código generado}\n"
    "\\begin{lstlisting}[style=mystyle]\n"
);

}

/**
 * Creates the epilogue of the generated output, that is, the final lines that
 * completes a valid Latex document.
 */
static void _generateEpilogue(ComputedValue * values) {
	_output(0, "\\end{lstlisting}\n\n");
	_output(0, "\\section*{Resultado}\n");
	_output(0, "\\begin{tcolorbox}[colback=blue!5!white, colframe=blue!75!black, title=Resultado de evaluación]\n");
	_generateResults(values);
	_output(0, "\\end{tcolorbox}\n\n\\end{document}\n");

}


static void _generateResults(ComputedValue *values) {
	if(values == NULL){
		_output(0, "%s", "No results to display.\n");
		return;
	}
	if(values->next != NULL) {
		_generateResults(values->next);
		_output(0, "%s", "\\\\\n");
		printf("\n"); 
	}
	switch (values->type) {
		case EVALUATE_RESULT:
			_output(0, "evaluate(%s, %s) = ", values->evaluateStatement->formulaName, values->evaluateStatement->valuationName);
			printf("evaluate(%s, %s) = ", values->evaluateStatement->formulaName, values->evaluateStatement->valuationName);
			break;
		case ADEQUATE_RESULT:
			_output(0, "adequate(%s) = ", values->adequateStatement->opsetName);
			printf("adequate(%s) = ", values->adequateStatement->opsetName);
			break;
		default:
			break;
	}
	_output(0, "%s", values->result ? "true" : "false");
    printf("%s\n", values->result ? "true" : "false");
}


/**
 * Generates an indentation string for the specified level.
 */
static char * _indentation(const unsigned int level) {
	return indentation(_indentationCharacter, level, _indentationSize);
}

/**
 * Outputs a formatted string to standard output. The "fflush" instruction
 * allows to see the output even close to a failure, because it drops the
 * buffering.
 */
static void _output(const unsigned int indentationLevel, const char * const format, ...) {
	va_list arguments;
	va_start(arguments, format);
	char * indentation = _indentation(indentationLevel);
	char * effectiveFormat = concatenate(2, indentation, format);
	vfprintf(_outputFile, effectiveFormat, arguments);
	fflush(_outputFile);
	free(effectiveFormat);
	free(indentation);
	va_end(arguments);
}

/** PUBLIC FUNCTIONS */
void generate(CompilerState * compilerState) {
	logDebugging(_logger, "Generating final output...");
	_generatePrologue();
	_generateProgram(compilerState->abstractSyntaxtTree);
	_generateEpilogue(compilerState->result_values_list);
	logDebugging(_logger, "Generation is done.");

}
