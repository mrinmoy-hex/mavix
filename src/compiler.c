#include <stdio.h>
#include <stdlib.h>

#include "include/common.h"
#include "include/compiler.h"
#include "include/scanner.h"

typedef struct {
    Token current;
    Token previous;
    bool hadError;
    bool panicMode;
} Parser;


// enum struct of precedence level (from lowest to highest)
typedef enum {
    PREC_NONE,
    PREC_ASSIGNMENT,    // =
    PREC_OR,            // or
    PREC_AND,           // and
    PREC_EQUALITY,      // == !=
    PREC_COMPARISION,   // < > <= >=
    PREC_TERM,          // + -
    PREC_FACTOR,        // * /
    PREC_UNARY,         // ! -
    PREC_CALL,          // . ()
    PREC_PRIMARY
} Precedence;


typedef void (*ParseFn) ();

typedef struct {
    ParseFn prefix;
    ParseFn infix;
    Precedence precedence;
/**
 * @brief Structure representing a parsing rule.
 */
} ParseRule;




Parser parser;
Chunk* compilingChunk;


static Chunk* currentChunk() {
    return compilingChunk;
}


static void errorAt(Token* token, const char* message) {
    // if panic mode is active, return immediately
    if (parser.panicMode) return;
    parser.panicMode = true;    // activate panic mode to avoid cascading errors
    fprintf(stderr, "[line %d] Error", token->line);

    if (token->type == TOKEN_EOF) {
        fprintf(stderr, " at end");
    } else if (token->type == TOKEN_ERROR) {
        // Nothing
    } else {
        fprintf(stderr, " at '%.*s'", token->length, token->start);
    }

    fprintf(stderr, ": %s\n", message);
    parser.hadError = true;
}



static void error(const char* message) {
    errorAt(&parser.previous, message);
}



static void errorAtCurrent(const char* message) {
    errorAt(&parser.current, message);
}


/**
 * @brief Advances the current position in the input stream.
 *
 * This function is responsible for moving the current position
 * forward in the input stream. It is typically used in the context
 * of a compiler or interpreter to process the next character or token.
 */
static void advance() {
    parser.previous = parser.current;

    for (;;) {
        parser.current = scanToken();
        // if the token is valid, exit the loop
        if (parser.current.type != TOKEN_ERROR) break;

        // report the error and continue scanning
        errorAtCurrent(parser.current.start);
    }
}

/**
 * @brief Consumes a token of the specified type.
 *
 * This function checks if the current token matches the expected type.
 * If it does, the token is consumed. If it does not, an error message
 * is reported.
 *
 * @param type The expected type of the token to consume.
 * @param message The error message to report if the token type does not match.
 */
static void consume(TokenType type, const char* message) {
    if (parser.current.type == type) {
        advance();
        return;
    }

    errorAtCurrent(message);
}




static void emitByte(uint8_t byte) {
    writeChunk(currentChunk(), byte, parser.previous.line);
}


static void emitBytes(uint8_t byte1, uint8_t byte2) {
    emitByte(byte1);
    emitByte(byte2);
}


static void endCompiler() {
    emitReturn();
}



// infix parser for binary operations
static void binary() {
    TokenType operatorType = parser.previous.type;
    ParseRule* rule = getRule(operatorType);
    parsePrecedence((Precedence) (rule->precedence + 1));

    switch (operatorType) {
        case TOKEN_PLUS:        emitByte(OP_ADD); break;
        case TOKEN_MINUS:       emitByte(OP_SUBTRACT); break;
        case TOKEN_ASTERISK:    emitByte(OP_MULTIPLY); break;
        case TOKEN_SLASH:       emitByte(OP_DIVIDE); break;
        default:
            return;             // Unreachable.
    }
}



// compiling groupings
static void grouping() {
    expression();
    consume(TOKEN_RIGHT_PAREN, "Expect ')' after expression.");
}



// compiling number literals
static void number() {
    // convert string into float
    double value = strtod(parser.previous.start, NULL);
    emitConstant(value);
}



// compiling unary expression
static void unary() {
    TokenType operatorType = parser.previous.type;

    // Compile the operand.
    parsePrecedence(PREC_UNARY);

    // Emit the operator instruction
    switch (operatorType) {
        case TOKEN_MINUS: emitByte(OP_NEGATE); break;
        default: return;    // Unreachable
    }
}




/**
 * An array of ParseRule structures that define the parsing rules for the compiler.
 */
ParseRule rules[] = {
    [TOKEN_LEFT_PAREN]    = {grouping, NULL,   PREC_NONE},
    [TOKEN_RIGHT_PAREN]   = {NULL,     NULL,   PREC_NONE},
    [TOKEN_LEFT_BRACE]    = {NULL,     NULL,   PREC_NONE}, 
    [TOKEN_RIGHT_BRACE]   = {NULL,     NULL,   PREC_NONE},
    [TOKEN_COMMA]         = {NULL,     NULL,   PREC_NONE},
    [TOKEN_DOT]           = {NULL,     NULL,   PREC_NONE},
    [TOKEN_MINUS]         = {unary,    binary, PREC_TERM},
    [TOKEN_PLUS]          = {NULL,     binary, PREC_TERM},
    [TOKEN_SEMICOLON]     = {NULL,     NULL,   PREC_NONE},
    [TOKEN_SLASH]         = {NULL,     binary, PREC_FACTOR},
    [TOKEN_ASTERISK]      = {NULL,     binary, PREC_FACTOR},
    [TOKEN_BANG]          = {NULL,     NULL,   PREC_NONE},
    [TOKEN_BANG_EQUAL]    = {NULL,     NULL,   PREC_NONE},
    [TOKEN_EQUAL]         = {NULL,     NULL,   PREC_NONE},
    [TOKEN_EQUAL_EQUAL]   = {NULL,     NULL,   PREC_NONE},
    [TOKEN_GREATER]       = {NULL,     NULL,   PREC_NONE},
    [TOKEN_GREATER_EQUAL] = {NULL,     NULL,   PREC_NONE},
    [TOKEN_LESS]          = {NULL,     NULL,   PREC_NONE},
    [TOKEN_LESS_EQUAL]    = {NULL,     NULL,   PREC_NONE},
    [TOKEN_IDENTIFIER]    = {NULL,     NULL,   PREC_NONE},
    [TOKEN_STRING]        = {NULL,     NULL,   PREC_NONE},
    [TOKEN_NUMBER]        = {number,   NULL,   PREC_NONE},
    [TOKEN_AND]           = {NULL,     NULL,   PREC_NONE},
    [TOKEN_CLASS]         = {NULL,     NULL,   PREC_NONE},
    [TOKEN_ELSE]          = {NULL,     NULL,   PREC_NONE},
    [TOKEN_FALSE]         = {NULL,     NULL,   PREC_NONE},
    [TOKEN_FOR]           = {NULL,     NULL,   PREC_NONE},
    [TOKEN_FUN]           = {NULL,     NULL,   PREC_NONE},
    [TOKEN_IF]            = {NULL,     NULL,   PREC_NONE},
    [TOKEN_NIL]           = {NULL,     NULL,   PREC_NONE},
    [TOKEN_OR]            = {NULL,     NULL,   PREC_NONE},
    [TOKEN_PRINT]         = {NULL,     NULL,   PREC_NONE},
    [TOKEN_RETURN]        = {NULL,     NULL,   PREC_NONE},
    [TOKEN_SUPER]         = {NULL,     NULL,   PREC_NONE},
    [TOKEN_THIS]          = {NULL,     NULL,   PREC_NONE},
    [TOKEN_TRUE]          = {NULL,     NULL,   PREC_NONE},
    [TOKEN_VAR]           = {NULL,     NULL,   PREC_NONE},
    [TOKEN_WHILE]         = {NULL,     NULL,   PREC_NONE},
    [TOKEN_ERROR]         = {NULL,     NULL,   PREC_NONE},
    [TOKEN_EOF]           = {NULL,     NULL,   PREC_NONE},
};





static void parsePrecedence(Precedence precedence) {
    // What goes here?
}





static void expression() {
    parsePrecedence(PREC_ASSIGNMENT);
}


static void emitReturn() {
    emitByte(OP_RETURN);
}


// Adds a constant & returns its index
static uint8_t makeConstant(Value value) {
    int constant = addConstant(currentChunk(), value);

    if (constant <= UINT8_MAX) {
        return (uint8_t)constant;
    }

    if (constant > 0xFFFFFF) {  // 16,777,215 max index
        error("Too many constants in one chunk.");
        return 0;
    }

    return constant;  // We’ll handle this in emitConstant()
}



// Adds a constant and emits bytecode 
static void emitConstant(Value value) {
    // add constant to chunk's constant pool
    int constant = addConstant(currentChunk(), value);

    if (constant <= UINT8_MAX) {
        emitBytes(OP_CONSTANT, (uint8_t)constant);
    } else {
        emitByte(OP_CONSTANT_LONG);
        emitByte((constant >> 16) & 0xFF);      // high byte
        emitByte((constant >> 8) & 0xFF);       // middle byte
        emitByte(constant & 0xFF);              // low byte
    }
}



bool compile(const char *source, Chunk* chunk) {
    initScanner(source);
    compilingChunk = chunk;

    parser.hadError = false;
    parser.panicMode = false;

    advance();
    expression();
    consume(TOKEN_EOF, "Expect end of expression.");
    endCompiler();
    
    return !parser.hadError;
}
