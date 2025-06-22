#include "backend/code-generation/Generator.h"
#include "backend/domain-specific/NonixComputer.h"
#include "frontend/lexical-analysis/FlexActions.h"
#include "frontend/syntactic-analysis/AbstractSyntaxTree.h"
#include "frontend/syntactic-analysis/BisonActions.h"
#include "frontend/syntactic-analysis/SyntacticAnalyzer.h"
#include "shared/CompilerState.h"
#include "shared/Environment.h"
#include "shared/Logger.h"
#include "shared/String.h"

/**
 * The main entry-point of the entire application. If you use "strtok" to
 * parse anything inside this project instead of using Flex and Bison, I will
 * find you, and I will kill you (Bryan Mills; "Taken", 2008).
 */
const int main(const int count, const char ** arguments) {
	Logger * logger = createLogger("EntryPoint");
	initializeFlexActionsModule();
	initializeBisonActionsModule();
	initializeSyntacticAnalyzerModule();
	initializeAbstractSyntaxTreeModule();
	initializeNonixComputerModule();
	initializeGeneratorModule();

	// Logs the arguments of the application.
	for (int k = 0; k < count; ++k) {
		logDebugging(logger, "Argument %d: \"%s\"", k, arguments[k]);
	}

	// Begin compilation process.
	CompilerState compilerState = {
		.abstractSyntaxtTree = NULL,
		.symbolTable = NULL,
		.succeed = false,
		.result_values_list = NULL
	};

	const SyntacticAnalysisStatus syntacticAnalysisStatus = parse(&compilerState);

	CompilationStatus compilationStatus = SUCCEED;
	if (syntacticAnalysisStatus == ACCEPT) {
		// ----------------------------------------------------------------------------------------
		// Beginning of the Backend... ------------------------------------------------------------
		logDebugging(logger, "Computing expression value...");
		Program * program = compilerState.abstractSyntaxtTree;
		compilerState.result_values_list = computeProgram(program, compilerState.symbolTable, &compilerState.succeed);
		if (compilerState.succeed) {
			generate(&compilerState);
		}
		else {
			logError(logger, "The computation phase rejects the input program.");
			compilationStatus = FAILED;
		}
		// ...end of the Backend. -----------------------------------------------------------------
		// ----------------------------------------------------------------------------------------
	}
	else {
		logError(logger, "The syntactic-analysis phase rejects the input program.");
		compilationStatus = FAILED;
	}

	// Release resources.
	if(compilerState.abstractSyntaxtTree != NULL) {
		logDebugging(logger, "Releasing AST resources...");
		releaseProgram(compilerState.abstractSyntaxtTree);
	}

	if(compilerState.result_values_list != NULL) {
		logDebugging(logger, "Releasing results list resources...");
		free_results_list(compilerState.result_values_list);
	}
	if (compilerState.symbolTable != NULL) {
		logDebugging(logger, "Releasing symbol table resources...");
		free_symbol_table(&compilerState.symbolTable);
	}

	logDebugging(logger, "Releasing modules resources...");
	shutdownGeneratorModule();
	shutdownNonixComputerModule();
	shutdownAbstractSyntaxTreeModule();
	shutdownSyntacticAnalyzerModule();
	shutdownBisonActionsModule();
	shutdownFlexActionsModule();
	logDebugging(logger, "Compilation is done.");
	destroyLogger(logger);
	return compilationStatus;
}
