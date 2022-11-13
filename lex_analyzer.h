#ifndef LEX_ANALYZER_H
#define LEX_ANALYZER_H

#include "token_type.h"

Token getNextToken();

void printToken(Token*);

Token getOperator();
Token getInterger();
Token getReal();
Token getBracket();
Token getComma();
Token getBuiltinFunction();
Token getParserDirective();

struct _SymbolEntry {
    char name[11];
    Token token;
} typedef SymbolEntry;

int symbolTableIndex;
SymbolEntry symbolTable[100];
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
