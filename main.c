#include <stdio.h>
#include <stdlib.h>

#include "execute.h"
#include "lex_analyzer.h"
#include "parser.h"
#include "token_type.h"

extern ParseNode* last_syntax_tree;
Token* last_token_statement;
int main() {
    int len;
    int isEOF = 0;
    while (!isEOF) {
        Token* token_statement = malloc(sizeof(Token) * 100);

        printf("> ");
        getTokenStatement(token_statement, &len, &isEOF);
        if (isFailToken(token_statement[0])) continue;

        ParseNode* syntax_tree = getSyntaxTree(token_statement, len);
        ParseNode* copyed_syntax_tree = copyTree(syntax_tree);

        int error = interpret(syntax_tree);
        if (!error && syntax_tree) {
            if (last_syntax_tree) {
                free(last_token_statement);
                freeSyntaxTree(last_syntax_tree);
            }
            last_token_statement = token_statement;
            last_syntax_tree = copyed_syntax_tree;
        } else if (copyed_syntax_tree) {
            free(token_statement);
            freeSyntaxTree(copyed_syntax_tree);
        }
    }
}