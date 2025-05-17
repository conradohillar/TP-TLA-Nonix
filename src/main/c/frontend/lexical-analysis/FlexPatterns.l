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

"define"                            { return OnlyTokenLexemeAction(createLexicalAnalyzerContext(), DEFINE); }
"variable"                          { return OnlyTokenLexemeAction(createLexicalAnalyzerContext(), VARIABLE); }
"formula"                           { return OnlyTokenLexemeAction(createLexicalAnalyzerContext(), FORMULA); }
"valuation"                         { return OnlyTokenLexemeAction(createLexicalAnalyzerContext(), VALUATION); }
"operator"                          { return OnlyTokenLexemeAction(createLexicalAnalyzerContext(), OPERATOR); }
"opset"                             { return OnlyTokenLexemeAction(createLexicalAnalyzerContext(), OPSET); }
"evaluate"                          { return OnlyTokenLexemeAction(createLexicalAnalyzerContext(), EVALUATE); }
"adequate"                          { return OnlyTokenLexemeAction(createLexicalAnalyzerContext(), ADEQUATE); }
"otherwise"                         { return OnlyTokenLexemeAction(createLexicalAnalyzerContext(), OTHERWISE); }
"true"                              { return BooleanSemanticValueLexemeAction(createLexicalAnalyzerContext(), true); }
"false"                             { return BooleanSemanticValueLexemeAction(createLexicalAnalyzerContext(), false); }

"&"                                 { return OnlyTokenLexemeAction(createLexicalAnalyzerContext(), AND); }
"|"                                 { return OnlyTokenLexemeAction(createLexicalAnalyzerContext(), OR); }
"!"                                 { return OnlyTokenLexemeAction(createLexicalAnalyzerContext(), NOT); }
"=>"                                { return OnlyTokenLexemeAction(createLexicalAnalyzerContext(), THEN); }
"<=>"                               { return OnlyTokenLexemeAction(createLexicalAnalyzerContext(), IFF); }

[a-zA-Z_][a-zA-Z0-9_]*              { return IdentifierSemanticValueLexemeAction(createLexicalAnalyzerContext(), IDENTIFIER); }

"="                                 { return OnlyTokenLexemeAction(createLexicalAnalyzerContext(), EQUALS); }
"$"                                 { return OnlyTokenLexemeAction(createLexicalAnalyzerContext(), DOLLAR); }
"?"                                 { return OnlyTokenLexemeAction(createLexicalAnalyzerContext(), WILDCARD); }
"{"                                 { return OnlyTokenLexemeAction(createLexicalAnalyzerContext(), OPEN_BRACE); }
"}"                                 { return OnlyTokenLexemeAction(createLexicalAnalyzerContext(), CLOSE_BRACE); }
"("                                 { return OnlyTokenLexemeAction(createLexicalAnalyzerContext(), OPEN_PARENTHESIS); }
")"                                 { return OnlyTokenLexemeAction(createLexicalAnalyzerContext(), CLOSE_PARENTHESIS); }
","                                 { return OnlyTokenLexemeAction(createLexicalAnalyzerContext(), COMMA); }
";"                                 { return OnlyTokenLexemeAction(createLexicalAnalyzerContext(), SEMICOLON); }
"->"                                { return OnlyTokenLexemeAction(createLexicalAnalyzerContext(), ARROW); }

[[:space:]]+                        { IgnoredLexemeAction(createLexicalAnalyzerContext()); }

.                                   { return UnknownLexemeAction(createLexicalAnalyzerContext()); }

%%

#include "FlexExport.h"
