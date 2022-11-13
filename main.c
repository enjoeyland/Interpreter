#include <stdio.h>

#include "execute.h"
#include "lex_analyzer.h"
#include "parser.h"
#include "token_type.h"

extern TokenParser tp;
ParseNode* last_syntax_tree;
Token token_statement[100];
void interpret(Token* token_statement, int len) {
    // for (int i = 0; i < len; i++) {
    //     printf("%d ", token_statement[i].type);
    // }
    // printf("\n");

    tp = (TokenParser){token_statement, len, 0};
    ParseNode* syntax_tree = statement();

    last_syntax_tree = copyTree(syntax_tree);

    ParseNode* result = execute(syntax_tree);
    printf("%s\n", getTokenValue(result->current));
}

int main() {
    int len = 0;
    Token token;
    printf("> ");
    while (!isFailToken(token = getNextToken())) {
        if (token.type == ABSTRACT_SYNTAX_TREE) {
            print_syntax_tree(last_syntax_tree);
        } else if (token.type == SYMBOL_TABLE) {
            printSymbolTable();
        } else {
            token_statement[len++] = token;
            if (isNewlineToken(token)) {
                interpret(token_statement, len);
                len = 0;
                printf("> ");
            }
        }
    }
    token_statement[len++] = (Token){NEW_LINE, 0, NONE};
    interpret(token_statement, len);
}