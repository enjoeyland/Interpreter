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
    int child_num;
    struct _ParseNode* first;
    struct _ParseNode* second;
    struct _ParseNode* third;
} typedef ParseNode;

Token* getCurrentToken();

ParseNode* operator_add_sub();
ParseNode* operator_mul_div();

ParseNode* statement();
ParseNode* after_variable();
ParseNode* expression();
ParseNode* term();
ParseNode* factor();
ParseNode* unsigned_factor();
ParseNode* num();

void print_syntax_tree(ParseNode*);
ParseNode* copyTree(ParseNode*);
Token* syntax_error(char*);

#endif  // PARSER_H