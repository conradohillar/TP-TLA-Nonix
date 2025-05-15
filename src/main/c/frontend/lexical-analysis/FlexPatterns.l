%{

#include "FlexActions.h"

%}

/**
 * Enable start condition manipulation functions.
 */
%option stack

/**
 * Flex contexts (a.k.a. start conditions).
 *
 * @see https://westes.github.io/flex/manual/Start-Conditions.html
 */
%x MULTILINE_COMMENT

/**
 * Reusable patterns.
 *
 * @see https://westes.github.io/flex/manual/Matching.html
 * @see https://westes.github.io/flex/manual/Patterns.html
 */

%%

"/*"								{ BEGIN(MULTILINE_COMMENT); BeginMultilineCommentLexemeAction(createLexicalAnalyzerContext()); }
<MULTILINE_COMMENT>"*/"				{ EndMultilineCommentLexemeAction(createLexicalAnalyzerContext()); BEGIN(INITIAL); }
<MULTILINE_COMMENT>[[:space:]]+		{ IgnoredLexemeAction(createLexicalAnalyzerContext()); }
<MULTILINE_COMMENT>[^*]+			{ IgnoredLexemeAction(createLexicalAnalyzerContext()); }
<MULTILINE_COMMENT>.				{ IgnoredLexemeAction(createLexicalAnalyzerContext()); }

// Palabras clave
"define"                            { return KeywordLexemeAction(createLexicalAnalyzerContext(), DEFINE); }
"variable"                          { return KeywordLexemeAction(createLexicalAnalyzerContext(), VARIABLE); }
"formula"                           { return KeywordLexemeAction(createLexicalAnalyzerContext(), FORMULA); }
"valuation"                         { return KeywordLexemeAction(createLexicalAnalyzerContext(), VALUATION); }
"operator"                          { return KeywordLexemeAction(createLexicalAnalyzerContext(), OPERATOR); }
"opset"                             { return KeywordLexemeAction(createLexicalAnalyzerContext(), OPSET); }
"evaluate"                          { return KeywordLexemeAction(createLexicalAnalyzerContext(), EVALUATE); }
"adequate"                          { return KeywordLexemeAction(createLexicalAnalyzerContext(), ADEQUATE); }
"true"                              { return KeywordLexemeAction(createLexicalAnalyzerContext(), TRUE); }
"false"                             { return KeywordLexemeAction(createLexicalAnalyzerContext(), FALSE); }
"otherwise"                         { return KeywordLexemeAction(createLexicalAnalyzerContext(), OTHERWISE); }

// Conectivos
"&"                                 { return ConnectiveLexemeAction(createLexicalAnalyzerContext(), AND); }
"|"                                 { return ConnectiveLexemeAction(createLexicalAnalyzerContext(), OR); }
"!"                                 { return ConnectiveLexemeAction(createLexicalAnalyzerContext(), NOT); }
"=>"                                { return ConnectiveLexemeAction(createLexicalAnalyzerContext(), THEN); }
"<=>"                               { return ConnectiveLexemeAction(createLexicalAnalyzerContext(), IFF); }

// Identificadores
[a-zA-Z_][a-zA-Z0-9_]*              { return IdentifierLexemeAction(createLexicalAnalyzerContext()); }

// Símbolos
"="                                 { return SymbolLexemeAction(createLexicalAnalyzerContext(), EQUALS); }
"$"                                 { return SymbolLexemeAction(createLexicalAnalyzerContext(), DOLLAR); }
"?"                                 { return SymbolLexemeAction(createLexicalAnalyzerContext(), WILDCARD); }
"{"                                 { return SymbolLexemeAction(createLexicalAnalyzerContext(), OPEN_BRACE); }
"}"                                 { return SymbolLexemeAction(createLexicalAnalyzerContext(), CLOSE_BRACE); }
"("                                 { return SymbolLexemeAction(createLexicalAnalyzerContext(), OPEN_PARENTHESIS); }
")"                                 { return SymbolLexemeAction(createLexicalAnalyzerContext(), CLOSE_PARENTHESIS); }
","                                 { return SymbolLexemeAction(createLexicalAnalyzerContext(), COMMA); }
";"                                 { return SymbolLexemeAction(createLexicalAnalyzerContext(), SEMICOLON); }
"->"                                { return SymbolLexemeAction(createLexicalAnalyzerContext(), ARROW); }

// Espacios en blanco
[[:space:]]+                        { IgnoredLexemeAction(createLexicalAnalyzerContext()); }

// Cualquier otro carácter
.                                   { return UnknownLexemeAction(createLexicalAnalyzerContext()); }

%%

#include "FlexExport.h"
