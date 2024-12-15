#ifndef ERROR_H
#define ERROR_H
#include "lexer.h"

typedef enum {
  ERROR_WARNING,
  ERROR_ERROR
} bbc_error_severity_t;



void bbc_error_generic(bbc_error_severity_t severity, char *context_string);

void bbc_error_lexer(bbc_error_severity_t severity, bbc_lexer_t *lexer, bbc_lexer_token_t *token, char *context_string);

#endif // ERROR_H
