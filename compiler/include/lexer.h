#ifndef LEXER_H
#define LEXER_H

#include <stdbool.h>
#include <stddef.h>

/**
 * @brief Enumeration of token types used by the lexer
 *
 * @enum bbc_lexer_token_type_t
 */
typedef enum {

  TOKEN_END = 0,
  TOKEN_INVALID,
  TOKEN_KEYWORD,
  TOKEN_IDENTIFIER,
  TOKEN_NUMBER,
  TOKEN_PAREN_OPEN,
  TOKEN_PAREN_CLOSE,
  TOKEN_CURLY_OPEN,
  TOKEN_CURLY_CLOSE,
  TOKEN_SEMICOLON,
  TOKEN_COMMA,
  TOKEN_PLUS,
  TOKEN_PLUS_DOUBLE,
  TOKEN_MINUS,
  TOKEN_MINUS_DOUBLE,
  TOKEN_ASTERISK,
  TOKEN_SLASH_FORWARD,
  TOKEN_PERCENT,
  TOKEN_EQUALS,
  TOKEN_EQUALS_DOUBLE,
  TOKEN_BANG,
  TOKEN_BANG_EQUALS,
  TOKEN_LT,
  TOKEN_GT,
  TOKEN_LTE,
  TOKEN_GTE,
  TOKEN_AND_DOUBLE,
  TOKEN_BAR_DOUBLE,

} bbc_lexer_token_type_t;

/**
 * @brief Represents a position in source code with row and column coordinates
 *
 * @struct bbc_lexer_pos_t
 * @var row The line number in the source code (1-based)
 * @var col The column number in the source code (1-based)
 */
typedef struct {

  int row;
  int col;

} bbc_lexer_pos_t;

/**
 * @brief Represents a lexical token with its text, type and position
 *
 * @struct bbc_lexer_token_t
 * @var text a pointer to the first char of the token in the lexer content string
 * @var text_length The length of the token text
 * @var type The type of the token
 * @var pos The position of the token in the source code
 */
typedef struct {

  char *text;
  size_t text_length;

  bbc_lexer_token_type_t type;

  bbc_lexer_pos_t pos;
  
} bbc_lexer_token_t;

/**
 * @brief The source code content being lexed
 * @var content Pointer to the source code string
 * @var content_length Length of the source code content
 * @var cursor Current position in the content being processed
 * @var pos Current line and column position in the source
 */
typedef struct {
  char *content;
  size_t content_length;
  int cursor;
  bbc_lexer_pos_t pos;

} bbc_lexer_t;


/**
 * @brief Creates a new lexer instance for the given source content
 *
 * @param content Pointer to the source code string to be lexed
 * @param content_length Length of the source code content
 * @return bbc_lexer_t* Pointer to the newly created lexer instance
 *
 * @example
 * char *source = "int main() { return 0; }";
 * bbc_lexer_t *lexer = bbc_lexer_create(source, strlen(source));
 */
bbc_lexer_t *bbc_lexer_create(char *content, size_t content_length);

/**
 * @brief Gets the next token from the lexer's input stream
 *
 * @param lexer Pointer to the lexer instance
 * @return bbc_lexer_token_t The next token found in the input
 *
 * @example
 * bbc_lexer_t *lexer = bbc_lexer_create("x = 42;", 7);
 * bbc_lexer_token_t token = bbc_lexer_next_token(lexer);
 * // token will contain:
 * // - type: TOKEN_IDENTIFIER
 * // - text: "x"
 * // - pos: {row: 1, col: 1}
 */
bbc_lexer_token_t bbc_lexer_next_token(bbc_lexer_t *lexer);

/**
 * @brief Checks if the lexer's cursor position is within valid bounds
 *
 * @param lexer Pointer to the lexer instance
 * @return bool true if cursor is within content bounds, false otherwise
 *
 * @example
 * bbc_lexer_t *lexer = bbc_lexer_create("abc", 3);
 * bool is_safe = bbc_lexer_cursor_safe(lexer); // Returns true
 * lexer->cursor = 3;
 * is_safe = bbc_lexer_cursor_safe(lexer); // Returns false
 */
bool bbc_lexer_cursor_safe(bbc_lexer_t *lexer);

/**
 * @brief Increments the lexer's cursor position and updates line/column tracking
 *
 * @param lexer Pointer to the lexer instance
 *
 * @example
 * bbc_lexer_t *lexer = bbc_lexer_create("abc\ndef", 7);
 * bbc_lexer_cursor_inc(lexer); // Moves to 'b', pos = {row: 1, col: 2}
 * bbc_lexer_cursor_inc(lexer); // Moves to 'c', pos = {row: 1, col: 3}
 * bbc_lexer_cursor_inc(lexer); // Moves to '\n', pos = {row: 1, col: 4}
 * bbc_lexer_cursor_inc(lexer); // Moves to 'd', pos = {row: 2, col: 1}
 */
void bbc_lexer_cursor_inc(bbc_lexer_t *lexer);

/**
 * @brief Skips over whitespace characters in the lexer's input stream
 *
 * @param lexer Pointer to the lexer instance
 *
 * @example
 * bbc_lexer_t *lexer = bbc_lexer_create("   abc", 6);
 * bbc_lexer_trim(lexer); // Skips spaces, cursor now points to 'a'
 */
void bbc_lexer_trim(bbc_lexer_t *lexer);

/**
 * @brief Consumes and tokenizes a word (identifier or keyword) from the lexer input
 *
 * @param lexer Pointer to the lexer instance
 * @param token Pointer to the token structure to be populated
 *
 * @example
 * bbc_lexer_t *lexer = bbc_lexer_create("while (x)", 8);
 * bbc_lexer_token_t token;
 * bbc_lexer_consume_word(lexer, &token);
 * // token will contain:
 * // - type: TOKEN_KEYWORD
 * // - text: "while"
 * // - pos: {row: 1, col: 1}
 */
void bbc_lexer_consume_word(bbc_lexer_t *lexer, bbc_lexer_token_t *token);

/**
 * @brief Consumes and tokenizes a numeric literal from the lexer input
 *
 * @param lexer Pointer to the lexer instance
 * @param token Pointer to the token structure to be populated
 *
 * @example
 * bbc_lexer_t *lexer = bbc_lexer_create("42 + 3.14", 9);
 * bbc_lexer_token_t token;
 * bbc_lexer_consume_number(lexer, &token);
 * // token will contain:
 * // - type: TOKEN_NUMBER
 * // - text: "42"
 * // - pos: {row: 1, col: 1}
 */
void bbc_lexer_consume_number(bbc_lexer_t *lexer, bbc_lexer_token_t *token);

/**
 * @brief Consumes and tokenizes a symbol or operator from the lexer input
 *
 * @param lexer Pointer to the lexer instance
 * @param token Pointer to the token structure to be populated
 *
 * @example
 * bbc_lexer_t *lexer = bbc_lexer_create("+=42", 4);
 * bbc_lexer_token_t token;
 * bbc_lexer_consume_symbol(lexer, &token);
 * // token will contain:
 * // - type: TOKEN_PLUS
 * // - text: "+"
 * // - pos: {row: 1, col: 1}
 */
void bbc_lexer_consume_symbol(bbc_lexer_t *lexer, bbc_lexer_token_t *token);

/**
 * @brief Gets a string representation of a token's type
 *
 * @param token The token whose type name should be returned
 * @return char* String representation of the token type
 *
 * @example
 * bbc_lexer_t *lexer = bbc_lexer_create("42", 2);
 * bbc_lexer_token_t token = bbc_lexer_next_token(lexer);
 * char *name = bbc_lexer_get_token_name(token); // Returns "TOKEN_NUMBER"
 */
char *bbc_lexer_get_token_name(bbc_lexer_token_t token);

/**
 * @brief Prints debug information about a token to stdout
 *
 * @param token Pointer to the token to dump debug information for
 *
 * @example
 * bbc_lexer_t *lexer = bbc_lexer_create("x = 42;", 7);
 * bbc_lexer_token_t token = bbc_lexer_next_token(lexer);
 * bbc_lexer_debug_dump_token(&token);
 * // Prints something like:
 * // Token {type: TOKEN_IDENTIFIER, text: "x", pos: {1, 1}}
 */
void bbc_lexer_debug_dump_token(bbc_lexer_token_t *token);

#endif // LEXER_H
