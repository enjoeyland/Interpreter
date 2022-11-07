#include "parser.h"

#include <stdio.h>
#include <stdlib.h>

#include "token_type.h"

TokenParser tp;

// TODO: error 위치를 프린트 하기 위해서는 Token에 추가적인 정보를 갖고 있어야 한다.
Token* syntax_error() {
    printf("syntax error\n");
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
        if (type == COMMA || type == BRACKET_LEFT || type == BRACKET_RIGHT || type == NEW_LINE) {
            return NULL;
        }
        ParseNode* pn = malloc(sizeof(ParseNode));
        pn->current = getCurrentToken();
        pn->child_num = 0;
        tp.current++;
        return pn;
    } else {
        syntax_error();
    }
}

// operator
ParseNode* operator_add_sub() {
    if (isLookahead(PLUS)) {
        return match(PLUS);
    } else if (isLookahead(MINUS)) {
        return match(MINUS);
    } else {
        syntax_error();
    }
}
ParseNode* operator_mul_div() {
    if (isLookahead(MULTIPLY)) {
        return match(MULTIPLY);
    } else if (isLookahead(DIVIDE)) {
        return match(DIVIDE);
    } else {
        syntax_error();
    }
}

ParseNode* statement() {
    ParseNode* v = NULL;
    ParseNode* a = NULL;
    if (isLookahead(VARIABLE)) {
        v = match(VARIABLE);
        a = match(ASSIGN);
    }
    ParseNode* e = expression();
    match(NEW_LINE);
    ////////////////////////
    if (a == NULL) {
        return e;
    } else {
        a->child_num = 2;
        a->first = v;
        a->second = e;
        return a;
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
        ParseNode* n = num();
        ////////////////////////
        oas->child_num = 1;
        oas->first = n;
        return oas;
    } else if (isLookahead(STR)) {
        return match(STR);
    } else if (isLookahead(VARIABLE)) {
        return match(VARIABLE);
    } else {
        syntax_error();
    }
}
ParseNode* num() {
    if (isLookahead(INT)) {
        return match(INT);
    } else if (isLookahead(REAL)) {
        return match(REAL);
        match(REAL);
    }
}