#include <stdio.h>
#include <string.h>

#include "include/common.h"
#include "include/scanner.h"    

typedef struct {
    const char* start;
    const char* current;
    int line;
} Scanner;

Scanner scanner;


/**
 * @brief Initializes the scanner with the given source code.
 *
 * This function sets up the scanner to process the provided source code.
 *
 * @param source A pointer to a null-terminated string containing the source code to be scanned.
 */
void initScanner(const char* source) {
    scanner.start = source;
    scanner.current = source;
    scanner.line = 1;
}


static bool isAlpha(char c) {
    return (c >= 'a' && c <= 'z') ||
            (c >= 'A' && c <= 'Z') ||
            c == '_';
}



static bool isDigit(char c) {
    return c >= '0' && c <= '9';
}



/*
    If the current character is the null byte, then we’ve reached the end.
*/
static bool isAtEnd() {
    return *scanner.current == '\0';
}


static char advance() {
    scanner.current++;
    return scanner.current[-1];     // return the previous char
}

// Returns the current char, doesn't consume it
static char peek() {
    return *scanner.current;
}


static char peekNext() {
    if (isAtEnd()) return '\0';
    // return *(scanner.current + 1)
    return scanner.current[1];
}


static bool match(char expected) {
    if (isAtEnd()) return false;
    if (*scanner.current != expected) return false;
    scanner.current++;  // advances the pointer if matches
    return true;
}


/**
 * @brief Creates a new token of the specified type.
 *
 * This function initializes a new token with the given type.
 *
 * @param type The type of the token to be created.
 * @return A new token of the specified type.
 */
static Token makeToken(TokenType type) {
    Token token;
    token.type = type;
    token.start = scanner.start;
    token.length = (int) (scanner.current - scanner.start);
    token.line = scanner.line;

    return token;
}

static Token errorToken(const char* message) {
    Token token;
    token.type = TOKEN_ERROR;
    token.start = message;
    token.length = (int) strlen(message);
    token.line = scanner.line;
    return token;
}


/**
 * @brief Skips over any whitespace characters in the input.
 *
 * This function advances the input pointer past any whitespace characters
 * (such as spaces, tabs, and newlines) until it encounters a non-whitespace
 * character or the end of the input.
 */
static void skipWhitespace() {
    for (;;) {
        char c = peek();
        switch (c) {
            case ' ':
            // also checks for carriage returns
            case '\r':
            case '\t':
                advance();
                break;

            // handle newlines
            case '\n':
                scanner.line++;
                advance();
                break;

            // handle single-line comments
            case '/':
                // single-line comments
                if (peekNext() == '/') {
                    // A comment goes until the end of the line
                    while (peek() != '\n' && !isAtEnd()) advance();
                }
                // multi-line comments 
                else if (peek() == '*') {
                    advance();      // consume '*'
                    advance();      // move past the '/'

                    while (!isAtEnd()) {    
                        if (peek() == '\n') scanner.line++;     // trace newlines

                        if (peek() == '*' && peekNext() == '/') {
                            advance();      // consume '*'
                            advance();      // consume '/'
                            break;          // exit the loop after finding '*/'
                        }
                        
                        advance();          // continue scanning inside the comment
                    }
                    
                    // unterminated comment error
                    if (isAtEnd()) {
                        // printf("Error: Unterminated multi-line comment error");
                        errorToken("Unterminated multiline comment error.");
                    }
                } else {
                        return;     // not a comment, return
                    }
                
                break;
            default:
                return;
        }
    }
}


static TokenType identifierType() {
    return TOKEN_IDENTIFIER;
}


/**
 * @brief Scans and returns the next identifier token from the input source.
 *
 * This function reads characters from the input source to form an identifier token.
 * Identifiers typically consist of alphanumeric characters and underscores, and
 * they represent variable names, function names, etc., in the source code.
 *
 * @return Token representing the scanned identifier.
 */
static Token identifier() {

    while (isAlpha(peek()) || isDigit(peek())) advance();   // consume the identifier
    return makeToken(identifierType());
}


/** 
  *  @note This scanner does not convert the value immediately. It only stores the raw text(lexeme)
  *  as it appears in the source code
**/

static Token number() {

    while (isDigit(peek())) advance();

    // look for a fractional part.
    if (peek() == '.' && isDigit(peekNext())) {
        // consume the '.'
        advance();

        while (isDigit(peek())) advance();
    }

    return makeToken(TOKEN_NUMBER);
}



static Token string() {

    while (peek() != '"' && !isAtEnd()) {
        if (peek() == '\n') scanner.line++;     // increments the linenumber in case of multiline strings.
        advance();
    } 

    // if handling edge cases for string
    if (isAtEnd()) return errorToken("Unterminated string.");

    // the closing quote
    advance();
    return makeToken(TOKEN_STRING);
}




/**
 * @brief Scans and returns the next token from the input source.
 *
 * This function reads characters from the input source and constructs
 * the next token to be processed by the lexer. It handles various types
 * of tokens including keywords, identifiers, literals, and operators.
 *
 * @return Token The next token from the input source.
 */
Token scanToken() {
    /**
     * @brief we are at the beginning of a new token when we enter the function. 
     * Thus, we set scanner.start to point to the current character so we remember where the 
     * lexeme we’re about to scan starts.
     */
    skipWhitespace();
    scanner.start = scanner.current;

    if (isAtEnd()) return makeToken(TOKEN_EOF);

    // We then consume the current character and return a token for it.

    char c = advance();

    // handling indentifiers
    if (isAlpha(c)) return identifier();

    // handling digits
    if (isDigit(c)) return number();

    switch (c) {
        case '(': return makeToken(TOKEN_LEFT_PAREN);
        case ')': return makeToken(TOKEN_RIGHT_PAREN);
        case '{': return makeToken(TOKEN_LEFT_BRACE);
        case '}': return makeToken(TOKEN_RIGHT_BRACE);
        case ';': return makeToken(TOKEN_SEMICOLON);
        case ',': return makeToken(TOKEN_COMMA);
        case '.': return makeToken(TOKEN_DOT);
        case '-': return makeToken(TOKEN_MINUS);
        case '+': return makeToken(TOKEN_PLUS);
        case '/': return makeToken(TOKEN_SLASH);
        case '*': return makeToken(TOKEN_ASTERISK);
    
        // making two-char punctuation
        case '!':
            return makeToken(
                match('=') ? TOKEN_BANG_EQUAL : TOKEN_BANG);
        case '=':
        return makeToken(
            match('=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL);
        case '<':
        return makeToken(
            match('=') ? TOKEN_LESS_EQUAL : TOKEN_EQUAL);
        case '>':
        return makeToken(
            match('=') ? TOKEN_GREATER_EQUAL : TOKEN_EQUAL);
        
        // check for strings
        case '"': return string();
        
    }

    

    return errorToken("Unexpected character.");

}


