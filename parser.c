#include "parser.h"

#include <stdio.h>
#include <stdlib.h>

#include "lex_analyzer.h"
#include "parsenode_queue.c"
#include "token_type.h"

TokenParser tp;
ParseNode* last_syntax_tree;
ParseNode failParsing = {&failToken, 0, NULL, NULL, NULL};

void syntax_error(char* msg) {
    printf("syntax error: %s\n", msg);
}

int isLookahead(TokenType type) {
    return tp.statement[tp.current].type == type;
}

Token* getCurrentToken() {
    return &tp.statement[tp.current];
}

ParseNode* match(TokenType type) {
    if (isLookahead(type)) {
        Token* t = getCurrentToken();
        tp.current++;

        if (type == COMMA || type == BRACKET_LEFT || type == BRACKET_RIGHT || type == NEW_LINE || type == ABSTRACT_SYNTAX_TREE || type == SYMBOL_TABLE) {
            return NULL;
        }
        ParseNode* pn = calloc(1, sizeof(ParseNode));
        pn->current = t;
        pn->child_num = 0;
        return pn;
    } else {
        char msg[100];
        sprintf(msg, "expected %s but %s are given", tokenTypeName[type], getTokenValue(getCurrentToken()));
        syntax_error(msg);
        exit(1);
    }
}

ParseNode* getSyntaxTree(Token* token_statement, int len) {
    tp = (TokenParser){token_statement, len, 0};
    ParseNode* syntax_tree = statement();
    return syntax_tree;
}

ParseNode* statement() {
    if (isLookahead(NEW_LINE)) {
        return NULL;
    } else if (isLookahead(ABSTRACT_SYNTAX_TREE)) {
        match(ABSTRACT_SYNTAX_TREE);
        match(NEW_LINE);
        ////////////////////////
        print_syntax_tree(last_syntax_tree);
        return NULL;
    } else if (isLookahead(SYMBOL_TABLE)) {
        match(SYMBOL_TABLE);
        match(NEW_LINE);
        ////////////////////////
        printSymbolTable();
        return NULL;
    }

    ParseNode* s = expression();
    while (isLookahead(ASSIGN)) {
        ParseNode* a = match(ASSIGN);
        ParseNode* s2 = sub_statement();
        ////////////////////////
        a->child_num = 2;
        a->first = s;
        a->second = s2;
        s = a;
    }
    match(NEW_LINE);
    ////////////////////////
    return s;
}
ParseNode* sub_statement() {
    ParseNode* s = expression();
    while (isLookahead(ASSIGN)) {
        ParseNode* a = match(ASSIGN);
        ParseNode* s2 = sub_statement();
        ////////////////////////
        a->child_num = 2;
        a->first = s;
        a->second = s2;
        s = a;
    }
    ////////////////////////
    return s;
}

ParseNode* expression() {
    ParseNode* e = term();
    while (isLookahead(PLUS) || isLookahead(MINUS)) {
        ParseNode* oas = operator_add_sub();
        ParseNode* t = term();
        ////////////////////////
        oas->child_num = 2;
        oas->first = e;
        oas->second = t;
        e = oas;
    }
    ////////////////////////
    return e;
}

ParseNode* term() {
    ParseNode* t = factor();
    while (isLookahead(MULTIPLY) || isLookahead(DIVIDE)) {
        ParseNode* omd = operator_mul_div();
        ParseNode* f = factor();
        ////////////////////////
        omd->child_num = 2;
        omd->first = t;
        omd->second = f;
        t = omd;
    }
    ////////////////////////
    return t;
}

ParseNode* factor() {
    if (isLookahead(BRACKET_LEFT)) {
        match(BRACKET_LEFT);
        ParseNode* e = expression();
        match(BRACKET_RIGHT);
        ////////////////////////
        return e;
    } else if (isLookahead(BUILTIN_SUB)) {
        ParseNode* sp = match(BUILTIN_SUB);
        match(BRACKET_LEFT);
        ParseNode* s = expression();
        match(COMMA);
        ParseNode* i1 = expression();
        match(COMMA);
        ParseNode* i2 = expression();
        match(BRACKET_RIGHT);
        ////////////////////////
        sp->child_num = 3;
        sp->first = s;
        sp->second = i1;
        sp->third = i2;
        return sp;
    } else if (isLookahead(PLUS) || isLookahead(MINUS)) {
        ParseNode* oas = operator_add_sub();
        ParseNode* uf = unary_factor();
        ////////////////////////
        oas->child_num = 1;
        oas->first = uf;
        return oas;
    } else if (isLookahead(INT) || isLookahead(REAL)) {
        return num();
    } else if (isLookahead(STR)) {
        return match(STR);
    } else if (isLookahead(VARIABLE)) {
        return match(VARIABLE);
    } else {
        char msg[100];
        sprintf(msg, "expected sub,-,+,int,real,str,variable but %s are given", getTokenValue(getCurrentToken()));
        syntax_error(msg);
        exit(1);
    }
}
ParseNode* unary_factor() {
    if (isLookahead(PLUS) || isLookahead(MINUS)) {
        ParseNode* oas = operator_add_sub();
        ParseNode* uf = unary_factor();
        ////////////////////////
        oas->child_num = 1;
        oas->first = uf;
        return oas;
    } else if (isLookahead(BRACKET_LEFT)) {
        match(BRACKET_LEFT);
        ParseNode* e = expression();
        match(BRACKET_RIGHT);
        ////////////////////////
        return e;
    } else if (isLookahead(INT) || isLookahead(REAL)) {
        return num();
    } else if (isLookahead(VARIABLE)) {
        return match(VARIABLE);
    } else {
        char msg[100];
        sprintf(msg, "expected +,-,(,int,real,variable but %s are given", getTokenValue(getCurrentToken()));
        syntax_error(msg);
        exit(1);
    }
}

ParseNode* num() {
    if (isLookahead(INT)) {
        return match(INT);
    } else if (isLookahead(REAL)) {
        return match(REAL);
    }
}

extern ParseNode* queue[MAX];
int print_syntax_tree_by_level() {
    ParseNode* node;
    while ((node = dequeue()) != NULL) {
        printf("%s", getTokenValue(node->current));
        if (node->child_num == 0) {
            printf(" ");
            continue;
        }
        printf("%d ", node->child_num);
        if (node->child_num >= 1) {
            enqueue(node->first);
        }
        if (node->child_num >= 2) {
            enqueue(node->second);
        }
        if (node->child_num == 3) {
            enqueue(node->third);
        }
    }
    printf("\n");
    if (!isEmpty()) {
        enqueue(NULL);
    }
}

// operator
ParseNode* operator_add_sub() {
    if (isLookahead(PLUS)) {
        return match(PLUS);
    } else if (isLookahead(MINUS)) {
        return match(MINUS);
    } else {
        syntax_error("oper + -");
        exit(1);
    }
}
ParseNode* operator_mul_div() {
    if (isLookahead(MULTIPLY)) {
        return match(MULTIPLY);
    } else if (isLookahead(DIVIDE)) {
        return match(DIVIDE);
    } else {
        syntax_error("oper * /");
        exit(1);
    }
}

void print_syntax_tree(ParseNode* pt) {
    if (pt == NULL) return;
    enqueue(pt);
    enqueue(NULL);
    while (!(isEmpty())) {
        print_syntax_tree_by_level();
    }
}

ParseNode* copyTree(ParseNode* pt) {
    if (pt == NULL) return NULL;
    ParseNode* new = malloc(sizeof(ParseNode));
    new->current = pt->current;
    new->child_num = pt->child_num;
    new->first = copyTree(pt->first);
    new->second = copyTree(pt->second);
    new->third = copyTree(pt->third);
    return new;
}

void freeSyntaxTree(ParseNode* syntax_tree) {
    if (syntax_tree == NULL) return;
    freeSyntaxTree(syntax_tree->first);
    freeSyntaxTree(syntax_tree->second);
    freeSyntaxTree(syntax_tree->third);
    free(syntax_tree);
}