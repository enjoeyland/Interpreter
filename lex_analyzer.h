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

int symbolTableIndex;
char symbolTable[100][11];
int getSymbolIdex(char*);
int getSymbolIdex_insert(char*);
void printSymbolTable();
Token getVariable();

int stringTableIndex;
char stringTable[100][100];
int getStringIdex(char*);
int getStringIdex_insert(char*);
void printStringTable();
Token getString();

#endif  // LEX_ANALYZER_H
