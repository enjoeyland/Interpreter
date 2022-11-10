#include "lex_analyzer.h"

#include <stdio.h>
#include <string.h>

#include "lex.yy.h"

Token getNextToken() {
    int token = yylex();
    if (token == 0) {
        return failToken;
    }
    switch (token) {
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
    return (Token){BUILTIN_SPLIT, 0, NONE};
}
Token getParserDirective() {
    char ast[] = "$ast";
    char symbol[] = "$symbol";
    if (strcmp(yytext, ast) == 0) {
        return (Token){BRACKET_LEFT, 0, NONE};
    } else if (strcmp(yytext, symbol) == 0) {
        return (Token){BRACKET_LEFT, 0, NONE};
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
    printf("index|\tsymbol\n");
    printf("----------------\n");
    for (int i = 0; i < symbolTableIndex; i++) {
        printf("%3d  |\t%s\n", i + 1, symbolTable[i].name);
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

void print_token(Token* t) {
    if (t == NULL) return;
    switch (t->type) {
        case VARIABLE:
            printf("%s", symbolTable[t->value.intValue - 1].name);
            break;
        case INT:
            printf("%d", t->value.intValue);
            break;
        case REAL:
            printf("%f", t->value.doubleValue);
            break;
        case STR:
            printf("%s", stringTable[t->value.intValue - 1]);
            break;
        case COMMA:
            printf(",");
            break;
        case NEW_LINE:
            printf("\\n");
            break;
        case BUILTIN_SPLIT:
            printf("split");
            break;
        case PLUS:
            printf("+");
            break;
        case MINUS:
            printf("-");
            break;
        case MULTIPLY:
            printf("*");
            break;
        case DIVIDE:
            printf("/");
            break;
        case ASSIGN:
            printf("=");
            break;
        case BRACKET_LEFT:
            printf("(");
            break;
        case BRACKET_RIGHT:
            printf(")");
            break;
    }
}