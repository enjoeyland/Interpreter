#include <stdio.h>

#include "execute.h"
#include "lex_analyzer.h"
#include "parser.h"
#include "token_type.h"

extern TokenParser tp;
void interpret(Token* token_statement, int len) {
    // for (int i = 0; i < len; i++) {
    //     printf("%d ", token_statement[i].type);
    // }
    // printf("\n");

    tp = (TokenParser){token_statement, len, 0};
    ParseNode* syntax_tree = statement();

    // printf("done make syntax tree\n");
    // print_syntax_tree(syntax_tree);
    // printf("done print syntax tree\n");

    ParseNode* result = execute(syntax_tree);
    printToken(result->current);
    printf("\n");
}

int main() {
    Token statement[100];
    int len = 0;
    Token token;
    printf("> ");
    while (!isFailToken(token = getNextToken())) {
        statement[len++] = token;
        if (isNewlineToken(token)) {
            interpret(statement, len);
            len = 0;
            printf("> ");
        }
    }
    statement[len++] = (Token){NEW_LINE, 0, NONE};
    interpret(statement, len);
}