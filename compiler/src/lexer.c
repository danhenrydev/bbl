#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"
#include "error.h"

bbc_lexer_t *bbc_lexer_create(char *content, size_t content_length) {

  bbc_lexer_t *lexer = malloc(sizeof(bbc_lexer_t));
  lexer->content = content;
  lexer->content_length = content_length;
  lexer->cursor = 0;
  lexer->pos.row = 1;
  lexer->pos.col = 1;

  return lexer;
}

bbc_lexer_token_t bbc_lexer_next_token(bbc_lexer_t *lexer) {

  bbc_lexer_trim(lexer);

  bbc_lexer_token_t token = {
    .text = NULL,
    .type = TOKEN_INVALID,
    .text_length = 0,
  };

  if(!bbc_lexer_cursor_safe(lexer)) {
    token.type = TOKEN_END;
  }

  if(bbc_lexer_cursor_safe(lexer) && isalpha(lexer->content[lexer->cursor])) {
    bbc_lexer_consume_word(lexer, &token);
    return token;
  }

  if(bbc_lexer_cursor_safe(lexer) && isdigit(lexer->content[lexer->cursor])) {
    bbc_lexer_consume_number(lexer, &token);
    return token;
  }

  bbc_lexer_consume_symbol(lexer, &token);
  if(token.type != TOKEN_INVALID) {
    bbc_lexer_cursor_inc(lexer);
    return token;
  }

  bbc_lexer_cursor_inc(lexer);
  
  return token;

}

bool bbc_lexer_cursor_safe(bbc_lexer_t *lexer) {

  if((size_t)lexer->cursor < lexer->content_length) {
    return true;
  }

  return false;
}

void bbc_lexer_cursor_inc(bbc_lexer_t *lexer) {
  lexer->cursor++;
  lexer->pos.col++;
}

void bbc_lexer_trim(bbc_lexer_t *lexer) {

  bool recurse = false;

  while(isspace(lexer->content[lexer->cursor]) && bbc_lexer_cursor_safe(lexer)) {
    if(lexer->content[lexer->cursor] == '\n') {
      lexer->pos.row++;
      lexer->pos.col = 0;
    }
    bbc_lexer_cursor_inc(lexer);
    recurse = true;
  }

  if(bbc_lexer_cursor_safe(lexer) && (lexer->content[lexer->cursor] == '/' && lexer->content[lexer->cursor + 1] == '/')) {
    while(lexer->content[lexer->cursor] != '\n' && bbc_lexer_cursor_safe(lexer)) {

      bbc_lexer_cursor_inc(lexer);
      recurse = true;

    }
  }

  if(recurse) {
    bbc_lexer_trim(lexer);
  }

  

}

void bbc_lexer_consume_word(bbc_lexer_t *lexer, bbc_lexer_token_t *token) {

  token->type = TOKEN_IDENTIFIER;

  if(bbc_lexer_cursor_safe(lexer)) {

    token->text = &lexer->content[lexer->cursor];
    token->pos = lexer->pos;

  }

  while(bbc_lexer_cursor_safe(lexer) && isalnum(lexer->content[lexer->cursor]) == 1) {
    
    token->text_length++;
    bbc_lexer_cursor_inc(lexer);

  }

  char *keywords[] = {
    "if",
    "else",
    "while",
    "return",
    "int",
    "void"
  };

  for(size_t i = 0; i < (sizeof(keywords) / sizeof(keywords[0])); i++) {

    if(strncmp(keywords[i], token->text, token->text_length) == 0) {
      token->type = TOKEN_KEYWORD;
    }

  }

}

void bbc_lexer_consume_number(bbc_lexer_t *lexer, bbc_lexer_token_t *token) {

  if(bbc_lexer_cursor_safe(lexer)) {
    token->type = TOKEN_NUMBER;
    token->text = &lexer->content[lexer->cursor];
    token->pos = lexer->pos;
  }

  while(bbc_lexer_cursor_safe(lexer) && isdigit(lexer->content[lexer->cursor])) {

    token->text_length++;
    bbc_lexer_cursor_inc(lexer);

  }

}

void bbc_lexer_consume_symbol(bbc_lexer_t *lexer, bbc_lexer_token_t *token) {


  token->pos = lexer->pos;
  token->text = &lexer->content[lexer->cursor];

  char symbols[] = {
    '(',
    ')',
    '{',
    '}',
    ';',
    ',',
    '%',
    '/',
    '!',
    '=',
    '>',
    '<',
    '&',
    '|',
    '+',
    '-',
    '*',
  };

  if(bbc_lexer_cursor_safe(lexer)){
    token->text_length = 1;
    for(size_t i = 0; i < (sizeof(symbols) / sizeof(symbols[0])); i++) {
      if(lexer->content[lexer->cursor] == symbols[i]) {

        switch(lexer->content[lexer->cursor]) {

          case '(':
            token->type = TOKEN_PAREN_OPEN;
          break;
          case ')':
            token->type = TOKEN_PAREN_CLOSE;
          break;
          case '{':
            token->type = TOKEN_CURLY_OPEN;
          break;
          case '}':
            token->type = TOKEN_CURLY_CLOSE;
          break;
          case ';':
            token->type = TOKEN_SEMICOLON;
          break;
          case ',':
            token->type = TOKEN_COMMA;
          break;
          case '%':
            token->type = TOKEN_PERCENT;
          break;
          case '/':
            token->type = TOKEN_SLASH_FORWARD;
          break;
          case '!':
            token->type = TOKEN_BANG;
            if(lexer->content[lexer->cursor + 1] == '=') {
              token->type = TOKEN_BANG_EQUALS;
              token->text_length = 2;
              lexer->cursor++;
            }
          break;
          case '=':
            token->type = TOKEN_EQUALS;
            if(lexer->content[lexer->cursor + 1] == '=') {
              token->type = TOKEN_EQUALS_DOUBLE;
              token->text_length = 2;
              lexer->cursor++;
            }
          break;
          case '>':
            token->type = TOKEN_GT;
            if(lexer->content[lexer->cursor + 1] == '=') {
              token->type = TOKEN_GTE;
              token->text_length = 2;
              lexer->cursor++;
            }
          break;
          case '<':
            token->type = TOKEN_LT;
            if(lexer->content[lexer->cursor + 1] == '=') {
              token->type = TOKEN_LTE;
              token->text_length = 2;
              lexer->cursor++;
            }
          break;
          case '&':
            if(lexer->content[lexer->cursor + 1] != '&') {
              bbc_error_lexer(ERROR_ERROR, lexer, token, "Use of a single '&' is invalid");
              exit(1);
            }
            token->type = TOKEN_AND_DOUBLE;
            token->text_length = 2;
            lexer->cursor++;
          break;
          case '|':
            if(lexer->content[lexer->cursor + 1] != '|') {
              bbc_error_lexer(ERROR_ERROR, lexer, token, "Use of a single '|' is invalid");
              exit(1);
            }
            token->type = TOKEN_BAR_DOUBLE;
            token->text_length = 2;
            lexer->cursor++;
          break;
          case '+':
            token->type = TOKEN_PLUS;
            if(lexer->content[lexer->cursor + 1] == '+') {
              token->type = TOKEN_PLUS_DOUBLE;
              token->text_length = 2;
              lexer->cursor++;
            }
          break;
          case '-':
            token->type = TOKEN_MINUS;
            if(lexer->content[lexer->cursor + 1] == '-') {
              token->type = TOKEN_MINUS_DOUBLE;
              token->text_length = 2;
              lexer->cursor++;
            }
          break;
          case '*':
            token->type = TOKEN_ASTERISK;
          break;

        }

      }
    }
  }
}

char *bbc_lexer_get_token_name(bbc_lexer_token_t token) {
  
  switch(token.type) {

    case TOKEN_END:
      return "TOKEN_END";
      break;
    case TOKEN_INVALID:
      return "TOKEN_INVALID";
      break;
    case TOKEN_KEYWORD:
      return "TOKEN_KEYWORD";
      break;
    case TOKEN_IDENTIFIER:
      return "TOKEN_IDENTIFIER";
      break;
    case TOKEN_NUMBER:
      return "TOKEN_NUMBER";
      break;
    case TOKEN_PAREN_OPEN:
      return "TOKEN_PAREN_OPEN";
      break;
    case TOKEN_PAREN_CLOSE:
      return "TOKEN_PAREN_CLOSE";
      break;
    case TOKEN_CURLY_OPEN:
      return "TOKEN_CURLY_OPEN";
      break;
    case TOKEN_CURLY_CLOSE:
      return "TOKEN_CURLY_CLOSE";
      break;
    case TOKEN_SEMICOLON:
      return "TOKEN_SEMICOLON";
      break;
    case TOKEN_COMMA:
      return "TOKEN_COMMA";
      break;
    case TOKEN_PLUS:
      return "TOKEN_PLUS";
      break;
    case TOKEN_PLUS_DOUBLE:
      return "TOKEN_PLUS_DOUBLE";
      break;
    case TOKEN_MINUS:
      return "TOKEN_MINUS";
      break;
    case TOKEN_MINUS_DOUBLE:
      return "TOKEN_MINUS_DOUBLE";
      break;
    case TOKEN_ASTERISK:
      return "TOKEN_ASTERISK";
      break;
    case TOKEN_SLASH_FORWARD:
      return "TOKEN_SLASH_FORWARD";
      break;
    case TOKEN_PERCENT:
      return "TOKEN_PERCENT";
      break;
    case TOKEN_EQUALS:
      return "TOKEN_EQUALS";
      break;
    case TOKEN_EQUALS_DOUBLE:
      return "TOKEN_EQUALS_DOUBLE";
      break;
    case TOKEN_BANG:
      return "TOKEN_BANG";
      break;
    case TOKEN_BANG_EQUALS:
      return "TOKEN_BANG_EQUALS";
      break;
    case TOKEN_LT:
      return "TOKEN_LT";
      break;
    case TOKEN_GT:
      return "TOKEN_GT";
      break;
    case TOKEN_LTE:
      return "TOKEN_LTE";
      break;
    case TOKEN_GTE:
      return "TOKEN_GTE";
      break;
    case TOKEN_AND_DOUBLE:
      return "TOKEN_AND_DOUBLE";
      break;
    case TOKEN_BAR_DOUBLE:
      return "TOKEN_BAR_DOUBLE";
      break;

  }

}

void bbc_lexer_debug_dump_token(bbc_lexer_token_t *token) {
  printf("DEBUG: Token '%.*s' (%s) - Line: %d Col %d\n", (int)token->text_length, token->text, bbc_lexer_get_token_name(*token), token->pos.row, token->pos.col);
}

