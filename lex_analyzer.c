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
    return (Token){REAL, .value.doubleValue = d, V_DOUBLE};
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

int symbolTableIndex = 0;
char symbolTable[100][11];

int getSymbolIdex(char* symbol) {
    for (int i = 0; i < symbolTableIndex; i++) {
        if (strcmp(symbol, symbolTable[i]) == 0) {
            return i + 1;
        }
    }
    return -1;
}

void printSymbolTable() {
    printf("\n");
    printf("index|\tsymbol\n");
    printf("----------------\n");
    for (int i = 0; i < symbolTableIndex; i++) {
        printf("%3d  |\t%s\n", i + 1, symbolTable[i]);
    }
}

Token getVariable() {
    int id_len = yyleng > 10 ? 10 : yyleng;

    char symbol[11];
    strncpy(symbol, yytext, id_len);
    symbol[id_len] = '\0';

    int index = getSymbolIdex(symbol);
    if (index == -1) {
        strncpy(symbolTable[symbolTableIndex++], symbol, id_len);
        index = symbolTableIndex;
    }
    return (Token){VARIABLE, index, V_INT};
}

int stringTableIndex = 0;
char stringTable[100][100];

int getStringIdex(char* symbol) {
    for (int i = 0; i < stringTableIndex; i++) {
        if (strcmp(symbol, stringTable[i]) == 0) {
            return i + 1;
        }
    }
    return -1;
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
    int index = getStringIdex(yytext);
    if (index == -1) {
        strncpy(stringTable[stringTableIndex++], yytext, yyleng);
        index = stringTableIndex;
    }
    return (Token){STR, index, V_INT};
}