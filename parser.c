#include "parser.h"

#include <stdio.h>

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
Token* match(int type) {
    if (isLookahead(type)) {
        Token* ct = getCurrentToken();
        tp.current++;
        return ct;
    } else {
        syntax_error();
    }
}

// operator
Token* operator_add_sub() {
    if (isLookahead(PLUS)) {
        return match(PLUS);
    } else if (isLookahead(MINUS)) {
        return match(MINUS);
    } else {
        syntax_error();
    }
}
void operator_mul_div() {
    if (isLookahead(MULTIPLY)) {
        match(MULTIPLY);
    } else if (isLookahead(DIVIDE)) {
        match(DIVIDE);
    } else {
        syntax_error();
    }
}

void statement() {
    if (isLookahead(VARIABLE)) {
        match(VARIABLE);
        match(ASSIGN);
    }
    expression();
}
void expression() {
    term();
    while (isLookahead(PLUS) || isLookahead(MINUS)) {
        operator_add_sub();
        term();
    }
}
void term() {
    factor();
    while (isLookahead(MULTIPLY) || isLookahead(DIVIDE)) {
        operator_mul_div();
        factor();
    }
}
void factor() {
    if (isLookahead(BRACKET_LEFT)) {
        match(BRACKET_LEFT);
        expression();
        match(BRACKET_RIGHT);
        ////////////////////////

    } else if (isLookahead(BUILTIN_SPLIT)) {
        match(BUILTIN_SPLIT);
        match(BRACKET_LEFT);
        expression();
        match(COMMA);
        expression();
        match(COMMA);
        expression();
        match(BRACKET_RIGHT);
    } else if (isLookahead(PLUS) || isLookahead(MINUS)) {
        operator_add_sub();
        num();
    } else if (isLookahead(STR)) {
        match(STR);
    } else if (isLookahead(VARIABLE)) {
        match(VARIABLE);
    } else {
        syntax_error();
    }
}
void num() {
    if (isLookahead(INT)) {
        match(INT);
    } else if (isLookahead(REAL)) {
        match(REAL);
    }
}