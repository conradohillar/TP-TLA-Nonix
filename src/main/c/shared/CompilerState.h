#ifndef COMPILER_STATE_HEADER
#define COMPILER_STATE_HEADER

#include "semantic-analysis/SymbolTable.h"

/**
 * The general status of a compilation.
*/
typedef enum {
	SUCCEED = 0,
	FAILED = 1
} CompilationStatus;

/**
 * The global state of the compiler. Should transport every data structure
 * needed across the different phases of a compilation.
 */
typedef struct {
	// The root node of the AST.
	void * abstractSyntaxtTree;

	// Symbol Table
	SymbolEntry * symbolTable;

	// A flag that indicates the current state of the compilation so far.
	boolean succeed;

	void * result_values_list; // List of computed values, if any.
} CompilerState;

#endif
