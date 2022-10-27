#include <stdio.h>

#include "Lexer.h"
#include "token_type.h"

int main() {
    int token;
    while ((token = yylex()) != 0) {
        if (token == OPERATOR) {
            printf("operator\n");
        } else if (token == INTEGER) {
            printf("interger\n");
        } else if (token == FLOAT) {
            printf("float\n");
        } else if (token == VARIABLE) {
            printf("variable\n");
        } else if (token == STRING) {
            printf("string\n");
        } else if (token == BRACKET) {
            printf("bracket\n");
        } else if (token == COMMA) {
            printf("comma\n");
        } else if (token == BUILTIN_FUCTION) {
            printf("built in function\n");
        } else if (token == PARSER_DIRECTIVE) {
            printf("parser directive\n");
        } else {
            printf("else\n");
        }
    }
}