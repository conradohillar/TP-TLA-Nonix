#include "AbstractSyntaxTree.h"

/* MODULE INTERNAL STATE */

static Logger * _logger = NULL;

void initializeAbstractSyntaxTreeModule() {
	_logger = createLogger("AbstractSyntxTree");
}

void shutdownAbstractSyntaxTreeModule() {
	if (_logger != NULL) {
		destroyLogger(_logger);
	}
}

/** PUBLIC FUNCTIONS */


void releaseProgram(Program * program) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (program != NULL) {
		releaseStatement(program->statement);
		releaseProgram(program->next);
		free(program);
	}
}

void releaseStatement(Statement * statement) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (statement != NULL) {
		switch (statement->type) {
			case DEFINE_VARIABLE:
				releaseDefineVariable(statement->defineVariable);
				break;
			case DEFINE_FORMULA:
				releaseDefineFormula(statement->defineFormula);
				break;
			case DEFINE_VALUATION:
				releaseDefineValuation(statement->defineValuation);
				break;
			case DEFINE_OPERATOR:
				releaseDefineOperator(statement->defineOperator);
				break;
			case DEFINE_OPSET:
				releaseDefineOpset(statement->defineOpset);
				break;
			case EVALUATE_STATEMENT:
				releaseEvaluateStatement(statement->evaluateStatement);
				break;
			case ADEQUATE_STATEMENT:
				releaseAdequateStatement(statement->adequateStatement);
				break;
		}
		free(statement);
	}
}

void releaseExpression(Expression * expression){
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (expression != NULL) {
		switch (expression->type) {
			case NOT_EXPRESSION:
				releaseNotExpression(expression->notExpression);
				break;
			case BINARY_EXPRESSION:
				releaseBinaryExpression(expression->binaryExpression);
				break;
			case CUSTOM_EXPRESSION:
				releaseCustomExpression(expression->customExpression);
				break;
			case VARIABLE_EXPRESSION:
				free(expression->variable);
				break;
		}
		free(expression);
	}
}

void releaseNotExpression(NotExpression * notExpression) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (notExpression != NULL) {
		releaseExpression(notExpression->expression);
		free(notExpression);
	}
}

void releaseBinaryExpression(BinaryExpression * binaryExpression) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (binaryExpression != NULL) {
		releaseExpression(binaryExpression->leftExpression);
		releaseExpression(binaryExpression->rightExpression);
		free(binaryExpression);
	}
}

void releaseCustomExpression(CustomExpression * customExpression) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (customExpression != NULL) {
		switch (customExpression->type) {
			case PREDEFINED_FORMULA:
				free(customExpression->predefinedFormula);
				break;
			case CUSTOM_OPERATOR:
				releaseCustomOperator(customExpression->customOperator);
				break;
		}
		free(customExpression);
	}
}

void releaseDefineVariable(DefineVariable * defineVariable) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (defineVariable != NULL) {
		releaseVariableList(defineVariable->variableList);
		free(defineVariable);
	}
}

void releaseVariableList(VariableList * variableList) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (variableList != NULL) {
		releaseVariableList(variableList->next);
		free(variableList->variable);
		free(variableList);
	}
}

void releaseDefineFormula(DefineFormula * defineFormula) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (defineFormula != NULL) {
		releaseExpression(defineFormula->expression);
		free(defineFormula->name);
		free(defineFormula);
	}
}

void releaseDefineValuation(DefineValuation * defineValuation) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (defineValuation != NULL) {
		releaseValuationList(defineValuation->valuationList);
		free(defineValuation);
	}
}

void releaseValuationList(ValuationList * valuationList) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (valuationList != NULL) {
		releaseValuationList(valuationList->next);
		releaseValuation(valuationList->valuation);
		free(valuationList);
	}
}

void releaseValuation(Valuation * valuation) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (valuation != NULL) {
		releaseTruthValue(valuation->truthValue);
		free(valuation->variable);
		free(valuation);
	}
}

void releaseDefineOpset(DefineOpset * defineOpset) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (defineOpset != NULL) {
		releaseOpsetList(defineOpset->opsetList);
		free(defineOpset->name);
		free(defineOpset);
	}
}

void releaseOpsetList(OpsetList * opsetList) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (opsetList != NULL) {
		releaseOpsetList(opsetList->next);
		free(opsetList);
	}
}

void releaseDefineOperator(DefineOperator * defineOperator) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (defineOperator != NULL) {
		releaseCustomOperator(defineOperator->customOperator);
		releaseTruthTable(defineOperator->truthTable);
		free(defineOperator);
	}
}

void releaseCustomOperator(CustomOperator * customOperator) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (customOperator != NULL) {
		releaseVariableList(customOperator->variableList);
		free(customOperator->name);
		free(customOperator);
	}
}

void releaseAdequateStatement(AdequateStatement * adequateStatement) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (adequateStatement != NULL) {
		free(adequateStatement->opsetName);
		free(adequateStatement);
	}
}

void releaseEvaluateStatement(EvaluateStatement * evaluateStatement) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (evaluateStatement != NULL) {
		free(evaluateStatement->formulaName);
		free(evaluateStatement->valuationName);
		free(evaluateStatement);
	}
}

void releaseTruthTable(TruthTable * truthTable) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (truthTable != NULL) {
		releaseTruthTable(truthTable->next);
		releaseTruthTableEntry(truthTable->entry);
		free(truthTable);
	}
}
void releaseTruthTableEntry(TruthTableEntry * truthTableEntry) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (truthTableEntry != NULL) {
		switch (truthTableEntry->type) {
			case TRUTH_VALUE_LIST:
				releaseTruthValueList(truthTableEntry->truthValueList);
				releaseTruthValue(truthTableEntry->mapValue);
				break;
			case OTHERWISE:
				releaseTruthValue(truthTableEntry->otherwiseValue);
				break;
		}
		free(truthTableEntry);
	}
}

void releaseTruthValueList(TruthValueList * truthValueList) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (truthValueList != NULL) {
		releaseTruthValueList(truthValueList->next);
		releaseTruthValueOrWildcard(truthValueList->truthValueOrWildcard);
		free(truthValueList);
	}
}

void releaseTruthValueOrWildcard(TruthValueOrWildcard * truthValueOrWildcard) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (truthValueOrWildcard != NULL) {
		switch (truthValueOrWildcard->type) {
			case TRUTH_VALUE:
				releaseTruthValue(truthValueOrWildcard->truthValue);
				break;
			case WILDCARD:
				// No need to free anything
				break;
		}
		free(truthValueOrWildcard);
	}
}

void releaseTruthValue(TruthValue * truthValue) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (truthValue != NULL) {
		free(truthValue);
	}
}
