#ifndef LEX_ANALYZER_H
#define LEX_ANALYZER_H

#include "token_type.h"

Token getNextToken();

void print_token(Token*);

Token getOperator();
Token getInterger();
Token getReal();
Token getBracket();
Token getComma();
Token getBuiltinFunction();
Token getParserDirective();

int getSymbolIdex(char*);
void printSymbolTable();
Token getVariable();

int getStringIdex(char*);
void printStringTable();
Token getString();

#endif  // LEX_ANALYZER_H
