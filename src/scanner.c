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
static inline bool isAtEnd() {
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


/**
 * @brief Checks if a given substring matches a specific keyword and returns the corresponding token type.
 *
 * This function verifies whether the identifier currently being scanned matches a predefined keyword.
 * It does this by:
 * 
 * 1. Ensuring the identifier's length matches the expected keyword length.
 * 2. Using `memcmp()` to check if the substring in the source code exactly matches the keyword.
 *
 * Unlike a Deterministic Finite Automaton (DFA), this function does not perform character-by-character 
 * state transitions. Instead, it performs an **optimized direct comparison** for keyword detection.
 *
 * @param start The starting index of the substring in the source code.
 * @param length The length of the substring to compare.
 * @param rest The keyword to compare the substring against.
 * @param type The token type to return if the substring matches the keyword.
 * @return The token type if the substring matches the keyword, otherwise TOKEN_IDENTIFIER.
 */
static TokenType checkKeyword(int start, int length, const char* rest, TokenType type) {
    /*
     * This function determines if the current identifier is a keyword.
     *
     * Condition 1:
     *   - The identifier's total length (scanner.current - scanner.start)
     *     must match the expected keyword length (start + length).
     *   - This prevents incorrect partial matches (e.g., "andrew" should not match "and").
     *
     * Condition 2:
     *   - `memcmp()` checks whether the substring (starting from `scanner.start + start`)
     *     is exactly the same as the keyword (`rest`) for `length` characters.
     *   - `memcmp()` ensures a **fast and direct byte-by-byte comparison**.
     *
     * If both conditions are met, the function returns the corresponding keyword token;
     * otherwise, it returns TOKEN_IDENTIFIER.
     */
    if (scanner.current - scanner.start == start + length &&
        memcmp(scanner.start + start, rest, length) == 0) {
        return type;
    }

    return TOKEN_IDENTIFIER;
}



/**
 * Determines the type of an identifier token.
 *
 * This function analyzes the current identifier and returns its corresponding
 * token type. It is used to differentiate between different types of identifiers
 * such as keywords, user-defined identifiers, etc.
 *
 * @return TokenType The type of the identifier token.
 */
static TokenType identifierType() {
    switch (scanner.start[0]) {
        case 'a': return checkKeyword(1, 2, "nd", TOKEN_AND);
        case 'c': return checkKeyword(1, 4, "lass", TOKEN_CLASS);
        case 'e': return checkKeyword(1, 3, "lse", TOKEN_ELSE);
        case 'f': 
        // check for 'false', 'for', 'fun'
            if (scanner.current - scanner.start > 1) {      // ensure atleast two character
                // checks the second character of false, for, fn
                switch (scanner.start[1]) {
                    case 'a': return checkKeyword(2, 3, "lse", TOKEN_FALSE);
                    case 'o': return checkKeyword(2, 1, "r", TOKEN_FOR);
                    case 'n': return checkKeyword(2, 0, "", TOKEN_FUN);
                }
            }
            break;
        case 'i': return checkKeyword(1, 1, "f", TOKEN_IF);
        case 'n': return checkKeyword(1, 3, "ull", TOKEN_NIL);
        case 'o': return checkKeyword(1, 1, "r", TOKEN_OR);
        case 'p': return checkKeyword(1, 6, "rintln", TOKEN_PRINT);
        case 'r': return checkKeyword(1, 5, "eturn", TOKEN_RETURN);
        case 's': return checkKeyword(1, 4, "uper", TOKEN_SUPER);
        case 't': 
            // check for 'this', 'true'
            if (scanner.current - scanner.start > 1) {
                switch (scanner.start[1]) {
                    case 'h': return checkKeyword(2, 2, "is", TOKEN_THIS);
                    case 'r': return checkKeyword(2, 2, "ue", TOKEN_TRUE);
                }
            }
            break;
        case 'v': return checkKeyword(1, 2, "ar", TOKEN_VAR);
        case 'w': return checkKeyword(1, 4, "hile", TOKEN_WHILE);
    }

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
            match('=') ? TOKEN_LESS_EQUAL : TOKEN_LESS);
        case '>':
        return makeToken(
            match('=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER);
        
        // check for strings
        case '"': return string();
        
    }


    return errorToken("Unexpected character.");

}
