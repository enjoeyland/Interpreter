#ifndef PARSER_H
#define PARSER_H

#include "token_type.h"

struct _TokenParser {
    Token* statement;
    int len;
    int current;
} typedef TokenParser;

struct _ParseNode {
    Token* current;
    Token* left;
    Token* right;
} typedef ParseNode;

Token* getCurrentToken();
Token* operator_add_sub();
void operator_mul_div();

void statement();
void expression();
void term();
void factor();
void num();

#endif  // PARSER_H