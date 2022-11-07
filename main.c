#include <stdio.h>

#include "lex_analyzer.h"
#include "parser.h"
#include "token_type.h"

extern TokenParser tp;
void interpret(Token* token_statement, int len) {
    for (int i = 0; i < len; i++) {
        printf("%d ", token_statement[i].type);
    }
    printf("\n");

    tp = (TokenParser){token_statement, len, 0};
    ParseNode* syntax_tree = statement();
    printf("made syntax tree\n");
    print_syntax_tree(syntax_tree);
}

int main() {
    Token statement[100];
    int len = 0;
    Token token;
    while (!isFailToken(token = getNextToken())) {
        statement[len++] = token;
        if (isNewlineToken(token)) {
            interpret(statement, len);
            len = 0;
        }
    }
}