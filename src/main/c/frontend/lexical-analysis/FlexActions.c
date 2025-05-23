#include "FlexActions.h"

/* MODULE INTERNAL STATE */

static Logger *_logger = NULL;
static boolean _logIgnoredLexemes = true;

void initializeFlexActionsModule() {
  _logIgnoredLexemes =
      getBooleanOrDefault("LOG_IGNORED_LEXEMES", _logIgnoredLexemes);
  _logger = createLogger("FlexActions");
}

void shutdownFlexActionsModule() {
  if (_logger != NULL) {
    destroyLogger(_logger);
  }
}

/* PRIVATE FUNCTIONS */

static void
_logLexicalAnalyzerContext(const char *functionName, LexicalAnalyzerContext *lexicalAnalyzerContext);

/**
 * Logs a lexical-analyzer context in DEBUGGING level.
 */
static void
_logLexicalAnalyzerContext(const char *functionName, LexicalAnalyzerContext *lexicalAnalyzerContext) {
  char *escapedLexeme = escape(lexicalAnalyzerContext->lexeme);
  logDebugging(_logger, "%s: %s (context = %d, length = %d, line = %d)",
               functionName, escapedLexeme,
               lexicalAnalyzerContext->currentContext,
               lexicalAnalyzerContext->length, lexicalAnalyzerContext->line);
              
  free(escapedLexeme);
}

/* PUBLIC FUNCTIONS */

void BeginMultilineCommentLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext) {
  if (_logIgnoredLexemes) {
    _logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
  }
  destroyLexicalAnalyzerContext(lexicalAnalyzerContext);
}

void EndMultilineCommentLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext) {
  if (_logIgnoredLexemes) {
    _logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
  }
  destroyLexicalAnalyzerContext(lexicalAnalyzerContext);
}

void IgnoredLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext) {
  if (_logIgnoredLexemes) {
    _logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
  }
  destroyLexicalAnalyzerContext(lexicalAnalyzerContext);
}

Token UnknownLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext) {
  _logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
  destroyLexicalAnalyzerContext(lexicalAnalyzerContext);
  return UNKNOWN;
}

Token OnlyTokenLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext, Token token) {
  _logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
  destroyLexicalAnalyzerContext(lexicalAnalyzerContext);
  return token;
}

Token IdentifierSemanticValueLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext, Token token) {
  _logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
  lexicalAnalyzerContext->semanticValue->keywordOrSymbol = lexicalAnalyzerContext->lexeme;
  destroyLexicalAnalyzerContext(lexicalAnalyzerContext);
  return token;
}

Token TrueSemanticValueLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext) {
  _logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
  lexicalAnalyzerContext->semanticValue->truth_value = true;
  destroyLexicalAnalyzerContext(lexicalAnalyzerContext);
  return TRUE;
}

Token FalseSemanticValueLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext) {
  _logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
  lexicalAnalyzerContext->semanticValue->truth_value = false;
  destroyLexicalAnalyzerContext(lexicalAnalyzerContext);
  return FALSE;
}
