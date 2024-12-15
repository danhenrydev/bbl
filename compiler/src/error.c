#include <stdio.h>
#include <stdlib.h>
#include "error.h"



void bbc_error_generic(bbc_error_severity_t severity, char *context_string) {

  switch(severity) {
    case ERROR_WARNING:
      printf("WARNING: ");
    break;
    case ERROR_ERROR:
      printf("ERROR: ");
    break;
  }

  printf("%s\n", context_string);

  if(severity == ERROR_ERROR) {
    exit(1);
  }

}

void bbc_error_lexer(bbc_error_severity_t severity, bbc_lexer_t *lexer, bbc_lexer_token_t *token, char *context_string) {

  switch(severity) {
    case ERROR_WARNING:
      printf("WARNING: ");
    break;
    case ERROR_ERROR:
      printf("ERROR: ");
    break;
  }

  printf("LEXER: Line %d:%d %s\n", token->pos.row, token->pos.col, context_string);

  int new_cursor = lexer->cursor;
  
  while(lexer->content[new_cursor - 1] != '\n') {
    new_cursor--;
  }

  int line_start = new_cursor;

  while(lexer->content[new_cursor + 1] != '\n') {
    new_cursor++;
  }

  printf("%.*s\n", new_cursor - line_start, &lexer->content[line_start]);

  for(int i = 1; i < token->pos.col; i++) {
    printf(" ");
  }

  printf("^ --- Offending Character\n");

  if(severity == ERROR_ERROR) {
    exit(1);
  }



}
