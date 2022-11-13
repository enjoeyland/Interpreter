#include "parser.h"

#include <stdio.h>
#include <stdlib.h>

#include "lex_analyzer.h"
#include "parsenode_queue.c"
#include "token_type.h"

TokenParser tp;

// TODO: error 위치를 프린트 하기 위해서는 Token에 추가적인 정보를 갖고 있어야 한다.
Token* syntax_error(char* msg) {
    printf("syntax error: %s\n", msg);
    exit(1);
    return getCurrentToken();
}

int isLookahead(int type) {
    return tp.statement[tp.current].type == type;
}

Token* getCurrentToken() {
    return &tp.statement[tp.current];
}

ParseNode* match(int type) {
    if (isLookahead(type)) {
        Token* t = getCurrentToken();
        tp.current++;

        if (type == COMMA || type == BRACKET_LEFT || type == BRACKET_RIGHT || type == NEW_LINE) {
            return NULL;
        }
        ParseNode* pn = malloc(sizeof(ParseNode));
        pn->current = t;
        pn->child_num = 0;
        return pn;
    } else {
        char msg[100];
        sprintf(msg, "match %d", type);
        syntax_error(msg);
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
    }
}
ParseNode* operator_mul_div() {
    if (isLookahead(MULTIPLY)) {
        return match(MULTIPLY);
    } else if (isLookahead(DIVIDE)) {
        return match(DIVIDE);
    } else {
        syntax_error("oper * /");
    }
}

ParseNode* statement() {
    if (isLookahead(NEW_LINE)) {
        return NULL;
    } else if (isLookahead(VARIABLE)) {
        ParseNode* v = match(VARIABLE);
        ParseNode* av = after_variable();
        ////////////////////////
        if (av == NULL) {
            return v;
        } else {
            av->first = v;
            return av;
        }
    } else {
        ParseNode* e = expression();
        match(NEW_LINE);
        ////////////////////////
        return e;
    }
}
ParseNode* after_variable() {
    if (isLookahead(ASSIGN)) {
        ParseNode* a = match(ASSIGN);
        ParseNode* e = statement();
        ////////////////////////
        a->child_num = 2;
        a->second = e;
        return a;
    } else if (isLookahead(NEW_LINE)) {
        match(NEW_LINE);
        return NULL;
    } else {
        syntax_error("after variable");
    }
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
    } else if (isLookahead(BUILTIN_SPLIT)) {
        ParseNode* sp = match(BUILTIN_SPLIT);
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
        ParseNode* uf = unsigned_factor();
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
        syntax_error("factor");
    }
}
ParseNode* unsigned_factor() {
    if (isLookahead(BRACKET_LEFT)) {
        match(BRACKET_LEFT);
        ParseNode* e = expression();
        match(BRACKET_RIGHT);
        ////////////////////////
        return e;
    } else if (isLookahead(INT) || isLookahead(REAL)) {
        return num();
    } else {
        syntax_error("unsigned factor");
    }
}

ParseNode* num() {
    if (isLookahead(INT)) {
        return match(INT);
    } else if (isLookahead(REAL)) {
        return match(REAL);
    } else {
        syntax_error("num");
    }
}

extern ParseNode* queue[MAX];
int print_syntax_tree_by_level() {
    ParseNode* node;
    while ((node = dequeue()) != NULL) {
        printToken(node->current);
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