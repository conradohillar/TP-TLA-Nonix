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

"define"                            { return TokenSemanticValueLexemeAction(createLexicalAnalyzerContext(), DEFINE); }
"variable"                          { return TokenSemanticValueLexemeAction(createLexicalAnalyzerContext(), VARIABLE); }
"formula"                           { return TokenSemanticValueLexemeAction(createLexicalAnalyzerContext(), FORMULA); }
"valuation"                         { return TokenSemanticValueLexemeAction(createLexicalAnalyzerContext(), VALUATION); }
"operator"                          { return TokenSemanticValueLexemeAction(createLexicalAnalyzerContext(), OPERATOR); }
"opset"                             { return TokenSemanticValueLexemeAction(createLexicalAnalyzerContext(), OPSET); }
"evaluate"                          { return TokenSemanticValueLexemeAction(createLexicalAnalyzerContext(), EVALUATE); }
"adequate"                          { return TokenSemanticValueLexemeAction(createLexicalAnalyzerContext(), ADEQUATE); }
"otherwise"                         { return TokenSemanticValueLexemeAction(createLexicalAnalyzerContext(), OTHERWISE); }
"true"                              { return BooleanSemanticValueLexemeAction(createLexicalAnalyzerContext(), true); }
"false"                             { return BooleanSemanticValueLexemeAction(createLexicalAnalyzerContext(), false); }

"&"                                 { return TokenSemanticValueLexemeAction(createLexicalAnalyzerContext(), AND); }
"|"                                 { return TokenSemanticValueLexemeAction(createLexicalAnalyzerContext(), OR); }
"!"                                 { return TokenSemanticValueLexemeAction(createLexicalAnalyzerContext(), NOT); }
"=>"                                { return TokenSemanticValueLexemeAction(createLexicalAnalyzerContext(), THEN); }
"<=>"                               { return TokenSemanticValueLexemeAction(createLexicalAnalyzerContext(), IFF); }

[a-zA-Z_][a-zA-Z0-9_]*              { return LiteralSemanticValueLexemeAction(createLexicalAnalyzerContext(), IDENTIFIER); }

"="                                 { return TokenSemanticValueLexemeAction(createLexicalAnalyzerContext(), EQUALS); }
"$"                                 { return TokenSemanticValueLexemeAction(createLexicalAnalyzerContext(), DOLLAR); }
"?"                                 { return TokenSemanticValueLexemeAction(createLexicalAnalyzerContext(), WILDCARD); }
"{"                                 { return TokenSemanticValueLexemeAction(createLexicalAnalyzerContext(), OPEN_BRACE); }
"}"                                 { return TokenSemanticValueLexemeAction(createLexicalAnalyzerContext(), CLOSE_BRACE); }
"("                                 { return TokenSemanticValueLexemeAction(createLexicalAnalyzerContext(), OPEN_PARENTHESIS); }
")"                                 { return TokenSemanticValueLexemeAction(createLexicalAnalyzerContext(), CLOSE_PARENTHESIS); }
","                                 { return TokenSemanticValueLexemeAction(createLexicalAnalyzerContext(), COMMA); }
";"                                 { return TokenSemanticValueLexemeAction(createLexicalAnalyzerContext(), SEMICOLON); }
"->"                                { return TokenSemanticValueLexemeAction(createLexicalAnalyzerContext(), ARROW); }

[[:space:]]+                        { IgnoredLexemeAction(createLexicalAnalyzerContext()); }

.                                   { return UnknownLexemeAction(createLexicalAnalyzerContext()); }

%%

#include "FlexExport.h"
