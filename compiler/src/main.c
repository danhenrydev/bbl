#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "error.h"
#include "lexer.h"

void usage() {

  printf("Usage:\n");
  printf("     bbc <filename>\n");

}

int main(int argc, char *argv[]) {

  if(argc < 2) {
    usage();
    exit(1);
  }

  char *content = bbc_read_file(argv[1]);
  if(content == NULL) {
    char error_message[128];
    sprintf(error_message, "Could not open file \"%s\"", argv[1]);
    bbc_error_generic(ERROR_ERROR, error_message);
  }
  static const bbc_lexer_token_t empty_token = {
    .type = TOKEN_END,
    .text_length = 0,
  };

  bbc_lexer_t *lexer = bbc_lexer_create(content, strlen(content));

  bbc_lexer_token_t token = bbc_lexer_next_token(lexer);

  while(token.type != TOKEN_END) {

    bbc_lexer_debug_dump_token(&token);
    token = empty_token;
    token = bbc_lexer_next_token(lexer);

  }



  return 0;
}

