#ifndef TOKEN_TYPE_H
#define TOKEN_TYPE_H

typedef enum _TokenClass {
    C_FAIL = -1,
    C_EOF,
    C_VARIABLE,
    C_INT,
    C_REAL,
    C_STR,
    C_COMMA,
    C_NEW_LINE,
    C_BUILTIN_FUCTION,
    C_OPERATOR,
    C_BRACKET,

    C_PARSER_DIRECTIVE
} TokenClass;

typedef enum _TokenType {
    FAIL = -1,
    T_EOF,
    VARIABLE,
    INT,
    REAL,
    STR,
    COMMA,
    NEW_LINE,

    BUILTIN_SUB,

    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE,
    ASSIGN,

    BRACKET_LEFT,
    BRACKET_RIGHT,

    ABSTRACT_SYNTAX_TREE,
    SYMBOL_TABLE
} TokenType;

#define NONE -1
#define V_INT 0
#define V_REAL 1

union Value {
    int intValue;
    double doubleValue;
};
struct _Token {
    TokenType type;
    union Value value;
    int valueType;
} typedef Token;
extern Token failToken;
extern char* tokenTypeName[17];

int isEqual(Token, Token);
int isFailToken(Token);
int isEOFToken(Token);
int isNewlineToken(Token);

#endif