#pragma once

typedef enum {
  // Single-character tokens.
  TOKEN_LPAREN, TOKEN_RPAREN,     // (  )
  TOKEN_LBRACE, TOKEN_RBRACE,     // {  }
  TOKEN_COMMA, TOKEN_DOT,         // ,  .
  TOKEN_MINUS, TOKEN_PLUS,        // -  +
  TOKEN_SEMICOLON, TOKEN_SLASH, TOKEN_ASTERISK,  // ;  /  *

  // One or two-character tokens.
  TOKEN_BANG, TOKEN_BANG_EQ,      // !  !=
  TOKEN_EQ, TOKEN_EQ_EQ,          // =  ==
  TOKEN_GT, TOKEN_GT_EQ,          // >  >=
  TOKEN_LT, TOKEN_LT_EQ,          // <  <=

  // Literals.
  TOKEN_IDENT,   // Identifier
  TOKEN_STR,     // String
  TOKEN_NUM,     // Number

  // Keywords.
  TOKEN_KW_AND, TOKEN_KW_CLASS, TOKEN_KW_ELSE, TOKEN_KW_FALSE,
  TOKEN_KW_FOR, TOKEN_KW_FUN, TOKEN_KW_IF, TOKEN_KW_NIL, TOKEN_KW_OR,
  TOKEN_KW_PRINT, TOKEN_KW_RETURN, TOKEN_KW_SUPER, TOKEN_KW_THIS,
  TOKEN_KW_TRUE, TOKEN_KW_VAR, TOKEN_KW_WHILE,

  // Special tokens.
  TOKEN_ERR,     // Error
  TOKEN_EOF      // End of file
} TokenType;



typedef struct {
    TokenType type;
    const char* start;
    int length;
    int line;
} Token;

void initScanner(const char* source);
Token scanToken();