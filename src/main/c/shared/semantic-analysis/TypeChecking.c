#include "TypeChecking.h"
#include <math.h>

static boolean has_otherwise_entry(TruthTable *truthTable);

unsigned int CheckTypeProgram(Program *program) {
    if(program == NULL) {
        return 0;
    }
    unsigned int result = 1;
    if (program->next != NULL) {
        result = CheckTypeProgram(program->next);
    }
    return result && CheckTypeStatement(program->statement);
}

unsigned int CheckTypeStatement(Statement *statement) {
    if (statement == NULL) {
        return 0;
    }
    switch (statement->type) {
        case DEFINE_VARIABLE:
            return 1;
        case DEFINE_FORMULA:
            return CheckTypeDefineFormula(statement->defineFormula);
        case DEFINE_VALUATION:
            return CheckTypeDefineValuation(statement->defineValuation);
        case DEFINE_OPERATOR:
            return CheckTypeDefineOperator(statement->defineOperator);
        case DEFINE_OPSET:
            return CheckTypeDefineOpset(statement->defineOpset);
        case EVALUATE_STATEMENT:
            return CheckTypeEvaluateStatement(statement->evaluateStatement);
        case ADEQUATE_STATEMENT:
            return CheckTypeAdequateStatement(statement->adequateStatement);
        default:
            return 0;
    }
}

unsigned int CheckTypeDefineFormula(DefineFormula *defineFormula) {
    return defineFormula != NULL && CheckTypeExpression(defineFormula->expression);
}

unsigned int CheckTypeExpression(Expression *expression) {
    if (expression == NULL) {
        return 0;
    }
    switch (expression->type) {
        case NOT_EXPRESSION:
            return CheckTypeNotExpression(expression->notExpression);
        case BINARY_EXPRESSION:
            return CheckTypeBinaryExpression(expression->binaryExpression);
        case CUSTOM_EXPRESSION:
            return CheckTypeCustomExpression(expression->customExpression);
        case VARIABLE_EXPRESSION:
            return CheckTypeVariable(expression->variable);
        default:
            return 0;
    }
}

unsigned int CheckTypeNotExpression(NotExpression *notExpression) {
    return notExpression != NULL && CheckTypeExpression(notExpression->expression);
}

unsigned int CheckTypeBinaryExpression(BinaryExpression *binaryExpression) {
    return binaryExpression != NULL &&
           CheckTypeExpression(binaryExpression->leftExpression) &&
           CheckTypeExpression(binaryExpression->rightExpression);
}

unsigned int CheckTypeCustomExpression(CustomExpression *customExpression) {
    if (customExpression == NULL) {
        return 0;
    }
    switch (customExpression->type) {
        case PREDEFINED_FORMULA:
            return CheckTypeFormula(customExpression->predefinedFormula);
        case CUSTOM_OPERATOR:
            return CheckTypeCustomOperator(customExpression->customOperator);
        default:
            return 0;
    }
}

unsigned int CheckTypeCustomOperator(CustomOperator *customOperator) {
    if (customOperator == NULL) {
        return 0;
    }
    return CheckTypeOperator(customOperator->name, list_size(customOperator->variableList)) &&
           CheckTypeVariableList(customOperator->variableList);
}

unsigned int CheckTypeDefineOperator(DefineOperator *defineOperator) {
    if (defineOperator == NULL) {
        return 0;
    }
    
    unsigned int args = list_size(defineOperator->customOperator->variableList);
    unsigned int table_entries = list_size(defineOperator->truthTable);

    unsigned int valid = 0;
    if (has_otherwise_entry(defineOperator->truthTable)){
        valid = table_entries <= pow(2, args); // Si hay otherwise, que el número de entradas sea a lo sumo 2^n
    } else {
        valid = table_entries == pow(2, args); // Si no, que el número de entradas sea exactamente 2^n
    }

    return valid && CheckTypeTruthTable(defineOperator->truthTable, args);
}

unsigned int CheckTypeTruthTable(TruthTable *truthTable, unsigned int args) {
    if (truthTable == NULL) {
        return 0;
    }
    unsigned int result = 1;
    if (truthTable->next != NULL) {
        result = CheckTypeTruthTable(truthTable->next, args);
    }
    return result && CheckTypeTruthTableEntry(truthTable->entry, args);
}

unsigned int CheckTypeTruthTableEntry(TruthTableEntry *truthTableEntry, unsigned int args) {
    if (truthTableEntry == NULL) {
        return 0;
    }
    switch (truthTableEntry->type) {
        case TRUTH_VALUE_LIST:{
            if(list_size(truthTableEntry->truthValueList) != args){
                return 0;
            }
            return check_truth_value_entry(&currentCompilerState()->truthValueMap, truthTableEntry->truthValueList, args);
        }
        case OTHERWISE_ENTRY:
            return 1;
        default:
            return 0;
    }
}

unsigned int CheckTypeDefineVariable(DefineVariable *defineVariable) {
    return defineVariable != NULL && CheckTypeVariableList(defineVariable->variableList);
}

unsigned int CheckTypeVariableList(VariableList *variableList) {
    unsigned int result = 1;
    if (variableList->next != NULL) { // Si el análisis sintáctico fue exitoso, no debería ser null el primer elemento de la lista
        result = CheckTypeVariableList(variableList->next);
    }
    return result && CheckTypeVariable(variableList->variable);
}

unsigned int CheckTypeDefineValuation(DefineValuation *defineValuation) {
    return defineValuation != NULL && CheckTypeValuationList(defineValuation->valuationList);
}

unsigned int CheckTypeValuationList(ValuationList *valuationList) {
    unsigned int result = 1;
    if (valuationList->next != NULL) { // Si el análisis sintático fue exitoso, no debería ser null el primer elemento de la lista
        result = CheckTypeValuationList(valuationList->next);
    }
    return result && CheckTypeValuation(valuationList->valuation);
}

unsigned int CheckTypeValuation(Valuation *valuation) {
    if(valuation == NULL) {
        return 0;
    }
    return CheckTypeVariable(valuation->variable);
}

unsigned int CheckTypeDefineOpset(DefineOpset *defineOpset) {
    return defineOpset != NULL && CheckTypeOpsetList(defineOpset->opsetList);
}

unsigned int CheckTypeOpsetList(OpsetList *opsetList) {
    unsigned int result = 1;
    if (opsetList->next != NULL) { // Si el análisis sintáctico fue exitoso, no debería ser null el primer elemento de la lista
        result = CheckTypeOpsetList(opsetList->next);
    }
    return result && CheckTypeOperator(opsetList->operator, -1); // No interesa el número de argumentos
}

unsigned int CheckTypeAdequateStatement(AdequateStatement *adequateStatement) {
    if(adequateStatement == NULL) {
        return 0;
    }
    return CheckTypeOpsetName(adequateStatement->opsetName);
}

unsigned int CheckTypeEvaluateStatement(EvaluateStatement *evaluateStatement) {
    if(evaluateStatement == NULL) {
        return 0;
    }
    return CheckTypeFormula(evaluateStatement->formulaName) &&
           CheckTypeValuationName(evaluateStatement->valuationName);
}

unsigned int CheckTypeVariable(Variable variable) {
    SymbolEntry *symbolEntry = find_symbol(currentCompilerState()->symbolTable, variable);
    return symbolEntry != NULL && symbolEntry->type == SYMBOL_VARIABLE;
}

unsigned int CheckTypeFormula(const char * formula) {
    SymbolEntry *symbolEntry = find_symbol(currentCompilerState()->symbolTable, formula);
    return symbolEntry != NULL && symbolEntry->type == SYMBOL_FORMULA;
}

unsigned int CheckTypeOperator(Operator operatorName, int count) {
    SymbolEntry *symbolEntry = find_symbol(currentCompilerState()->symbolTable, operatorName);
    if(symbolEntry == NULL) {
        return strcmp(operatorName, "&") == 0
            || strcmp(operatorName, "|") == 0
            || strcmp(operatorName, "=>") == 0
            || strcmp(operatorName, "<=>") == 0
            || strcmp(operatorName, "!") == 0;
    }
    return symbolEntry->type == SYMBOL_OPERATOR && ((count < 0) ? 1 : (symbolEntry->data.operator_data.argc == count)); // Decisión de diseño: si el count es -1, no se verifica el número de argumentos
}

unsigned int CheckTypeValuationName(const char * valuationName) {
    SymbolEntry *symbolEntry = find_symbol(currentCompilerState()->symbolTable, valuationName);
    return symbolEntry != NULL && symbolEntry->type == SYMBOL_VALUATION;
}

unsigned int CheckTypeOpsetName(const char * opsetName) {
    SymbolEntry *symbolEntry = find_symbol(currentCompilerState()->symbolTable, opsetName);
    return symbolEntry != NULL && symbolEntry->type == SYMBOL_OPSET;
}

unsigned int list_size(void * list) {
    unsigned int size = 0;
    if (list != NULL) {
        size++;
        if (((VariableList *)list)->next != NULL) {
            size += list_size(((VariableList *)list)->next);
        }
    }
    return size;
}

static boolean has_otherwise_entry(TruthTable *truthTable) {
    if (truthTable == NULL) {
        return false;
    }
    if (truthTable->entry->type == OTHERWISE_ENTRY) {
        return true;
    }
    return has_otherwise_entry(truthTable->next);
}