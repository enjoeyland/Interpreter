#include "lex_analyzer.h"

#include <stdio.h>
#include <string.h>

#include "lex.yy.h"

// TODO: 위치 print
void lexical_error(char* msg) {
    printf("lexical error: %s\n", msg);
}

void getTokenStatement(Token* token_statement, int* len, int* eof) {
    *len = 0;
    Token token;
    while (!isNewlineToken(token = getNextToken()) && !isEOFToken(token)) {
        if (isTypeOf(&token, FAIL)) {
            token_statement[0] = failToken;
            *len = 1;
            while (!isNewlineToken(token = getNextToken()) && !isEOFToken(token)) {
            }
            break;
        } else {
            token_statement[(*len)++] = token;
        }
    }
    if (isEOFToken(token)) {
        *eof = 1;
    }
    token_statement[(*len)++] = (Token){NEW_LINE, 0, NONE};
}

Token getNextToken() {
    int token = yylex();
    switch (token) {
        case C_FAIL:
            lexical_error(yytext);
            return failToken;
        case C_EOF:
            return (Token){T_EOF, 0, NONE};
        case C_OPERATOR:
            return getOperator();
        case C_INT:
            return getInterger();
        case C_REAL:
            return getReal();
        case C_VARIABLE:
            return getVariable();
        case C_STR:
            return getString();
        case C_BRACKET:
            return getBracket();
        case C_COMMA:
            return (Token){COMMA, 0, NONE};
        case C_BUILTIN_FUCTION:
            return getBuiltinFunction();
        case C_PARSER_DIRECTIVE:
            return getParserDirective();
        case C_NEW_LINE:
            return (Token){NEW_LINE, 0, NONE};
        default:
            printf("else\n");
            return failToken;
    }
}

Token getOperator() {
    char c = yytext[0];
    switch (c) {
        case '+':
            return (Token){PLUS, 0, NONE};
        case '-':
            return (Token){MINUS, 0, NONE};
        case '*':
            return (Token){MULTIPLY, 0, NONE};
        case '/':
            return (Token){DIVIDE, 0, NONE};
        case '=':
            return (Token){ASSIGN, 0, NONE};
    }
}
Token getInterger() {
    int i = atoi(yytext);
    return (Token){INT, i, V_INT};
}

Token getReal() {
    char* eptr;
    double d = strtod(yytext, &eptr);
    return (Token){REAL, .value.doubleValue = d, V_REAL};
}

Token getBracket() {
    char c = yytext[0];
    switch (c) {
        case '(':
            return (Token){BRACKET_LEFT, 0, NONE};
        case ')':
            return (Token){BRACKET_RIGHT, 0, NONE};
    }
}
Token getComma() {
    return (Token){COMMA, 0, NONE};
}
Token getBuiltinFunction() {
    return (Token){BUILTIN_SUB, 0, NONE};
}
Token getParserDirective() {
    char ast[] = "$ast";
    char symbol[] = "$symbol";
    if (strcmp(yytext, ast) == 0) {
        return (Token){ABSTRACT_SYNTAX_TREE, 0, NONE};
    } else if (strcmp(yytext, symbol) == 0) {
        return (Token){SYMBOL_TABLE, 0, NONE};
    }
}

int getSymbolIdex(char* symbol) {
    for (int i = 0; i < symbolTableIndex; i++) {
        if (strcmp(symbol, symbolTable[i].name) == 0) {
            return i + 1;
        }
    }
    return -1;
}
int getSymbolIdex_insert(char* symbol) {
    int len = strlen(symbol);
    int id_len = len > 10 ? 10 : len;

    char limited_symbol[11];
    strncpy(limited_symbol, symbol, id_len);
    limited_symbol[id_len] = '\0';

    int index = getSymbolIdex(limited_symbol);
    if (index == -1) {
        strncpy(symbolTable[symbolTableIndex++].name, limited_symbol, id_len);
        index = symbolTableIndex;
    }
    return index;
}

void printSymbolTable() {
    printf("\n");
    printf("index| %10s| %8s|\tvalue\n", "symbol", "type");
    printf("---------------------------------------\n");
    for (int i = 0; i < symbolTableIndex; i++) {
        SymbolEntry se = symbolTable[i];
        printf("%3d  | %10s| %8s|\t%s\n", i + 1, se.name, tokenTypeName[se.token.type], getTokenValue(&se.token));
    }
}

Token getVariable() {
    return (Token){VARIABLE, getSymbolIdex_insert(yytext), V_INT};
}

int getStringIdex(char* str) {
    for (int i = 0; i < stringTableIndex; i++) {
        if (strcmp(str, stringTable[i]) == 0) {
            return i + 1;
        }
    }
    return -1;
}

int getStringIdex_insert(char* str) {
    int index = getStringIdex(str);
    if (index == -1) {
        strncpy(stringTable[stringTableIndex++], str, strlen(str));
        index = stringTableIndex;
    }
    return index;
}

void printStringTable() {
    printf("\n");
    printf("index|\tstring\n");
    printf("----------------\n");
    for (int i = 0; i < stringTableIndex; i++) {
        printf("%3d  |\t%s\n", i + 1, stringTable[i]);
    }
}

Token getString() {
    return (Token){STR, getStringIdex_insert(yytext), V_INT};
}

char* getTokenValue(Token* t) {
    if (t == NULL) return "";
    char* result = malloc(sizeof(char) * 100);
    switch (t->type) {
        case FAIL:
            break;
        case VARIABLE:
            sprintf(result, "%s", symbolTable[t->value.intValue - 1].name);
            break;
        case INT:
            sprintf(result, "%d", t->value.intValue);
            break;
        case REAL:
            sprintf(result, "%f", t->value.doubleValue);
            break;
        case STR:
            sprintf(result, "%s", stringTable[t->value.intValue - 1]);
            break;
        default:
            sprintf(result, "%s", tokenTypeName[t->type]);
            break;
    }
    return result;
}