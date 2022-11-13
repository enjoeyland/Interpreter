#include <stdio.h>
#include <stdlib.h>

#include "execute.h"
#include "lex_analyzer.h"
#include "parser.h"
#include "token_type.h"

int main() {
    int len;
    int isEOF = 0;
    while (!isEOF) {
        Token* token_statement = malloc(sizeof(Token) * 100);
        printf("> ");
        getTokenStatement(token_statement, &len, &isEOF);
        if (isFailToken(token_statement[0])) {
            continue;
        }
        ParseNode* syntax_tree = getSyntaxTree(token_statement, len);
        interpret(syntax_tree);
    }
}