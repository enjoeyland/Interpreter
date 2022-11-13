#ifndef TOKEN_TYPE_H
#define TOKEN_TYPE_H

typedef enum _TokenClass {
    C_FAIL,
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
    FAIL,
    VARIABLE,
    INT,
    REAL,
    STR,
    COMMA,
    NEW_LINE,

    BUILTIN_SPLIT,

    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE,
    ASSIGN,

    BRACKET_LEFT,
    BRACKET_RIGHT
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

int isEqual(Token t1, Token t2);
int isFailToken(Token t);
int isNewlineToken(Token t);

#endif