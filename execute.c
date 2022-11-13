#include "execute.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lex_analyzer.h"
#include "parser.h"
#include "token_type.h"

void runtime_error(char* msg) {
    printf("runtime error: %s\n", msg);
    exit(1);
}

void interpret(ParseNode* syntax_tree) {
    ParseNode* result = execute(syntax_tree);
    if (result->current) {
        printf("%s\n", getTokenValue(result->current));
    }
}

ParseNode* execute(ParseNode* syntax_tree) {
    if (syntax_tree == NULL) {
        ParseNode* pn = malloc(sizeof(ParseNode));
        pn->current = NULL;
        return pn;
    }

    TokenType token_type;
    switch (token_type = syntax_tree->current->type) {
        case VARIABLE:
            return solveVariable(syntax_tree->current);
        case INT:
        case REAL:
        case STR:
            return syntax_tree;
        case PLUS:
        case MINUS:
        case MULTIPLY:
        case DIVIDE:
        case BUILTIN_SUB:
            return operate(token_type, execute(syntax_tree->first), execute(syntax_tree->second), execute(syntax_tree->third));
        case ASSIGN:
            return operateAssign(syntax_tree->first->current, execute(syntax_tree->second)->current);
    }
}

ParseNode* operate(TokenType token_type, ParseNode* first, ParseNode* second, ParseNode* third) {
    ParseNode* result;
    switch (token_type) {
        case PLUS:
            result = operatePlus(first->current, second->current);
            break;
        case MINUS:
            result = operateMinus(first->current, second->current);
            break;
        case MULTIPLY:
            result = operateMultiply(first->current, second->current);
            break;
        case DIVIDE:
            result = operateDivide(first->current, second->current);
            break;
        case BUILTIN_SUB:
            result = operateSub(first->current, second->current, third->current);
            break;
    }
    free(first);
    free(second);
    free(third);
    return result;
}

char* substring(char* str, int start, int len) {
    char* s = malloc(sizeof(char) * (len + 1));
    strncpy(s, str + start, len);
    return s;
}

char* removeDoubleQuote(char* str) {
    return substring(str, 1, strlen(str) - 2);
}

char* addDoubleQuote(char* str) {
    char* result = malloc(strlen(str) + 3);
    strcat(result, "\"");
    strcat(result, str);
    strcat(result, "\"");
    return result;
}

char* repeatStr(char* str, int num) {
    char* s = removeDoubleQuote(str);
    char* result = malloc(strlen(s) * num + 1);
    while (num-- > 0) {
        strcat(result, s);
    }
    return addDoubleQuote(result);
}

int divideStr(char* str1, char* str2) {
    char* s1 = removeDoubleQuote(str1);
    char* s2 = removeDoubleQuote(str2);

    int count = 0;
    while (strlen(s1) >= count * strlen(s2) && strncmp(s1 + count * strlen(s2), s2, strlen(s2)) == 0) {
        count++;
    }
    return count;
}

ParseNode* operatePlus(Token* operand1, Token* operand2) {
    Token* t = malloc(sizeof(Token));
    if (operand2 == NULL && (isTypeOf(operand1, INT) || isTypeOf(operand1, REAL))) {
        free(t);
        t = operand1;
    } else if (isTypeOf(operand1, INT) && isTypeOf(operand2, INT)) {
        t->type = INT;
        t->value.intValue = operand1->value.intValue + operand2->value.intValue;
        t->valueType = V_INT;
    } else if (isTypeOf(operand1, INT) && isTypeOf(operand2, REAL)) {
        t->type = REAL;
        t->value.doubleValue = operand1->value.intValue + operand2->value.doubleValue;
        t->valueType = V_REAL;
    } else if (isTypeOf(operand1, REAL) && isTypeOf(operand2, INT)) {
        t->type = REAL;
        t->value.doubleValue = operand1->value.doubleValue + operand2->value.intValue;
        t->valueType = V_REAL;
    } else if (isTypeOf(operand1, REAL) && isTypeOf(operand2, REAL)) {
        t->type = REAL;
        t->value.doubleValue = operand1->value.doubleValue + operand2->value.doubleValue;
        t->valueType = V_REAL;
    } else if (isTypeOf(operand1, STR) && isTypeOf(operand2, STR)) {
        t->type = STR;
        char* str1 = removeDoubleQuote(stringTable[operand1->value.intValue - 1]);
        char* str2 = removeDoubleQuote(stringTable[operand2->value.intValue - 1]);
        char* result = malloc(strlen(str1) + strlen(str2) + 1);
        strcat(result, str1);
        strcat(result, str2);
        result = addDoubleQuote(result);
        t->value.intValue = getStringIdex_insert(result);
        t->valueType = V_INT;
    } else if (isTypeOf(operand1, INT) && isTypeOf(operand2, STR)) {
        t->type = STR;
        char str1[20];
        sprintf(str1, "%d", operand1->value.intValue);
        char* str2 = removeDoubleQuote(stringTable[operand2->value.intValue - 1]);
        char* result = malloc(strlen(str1) + strlen(str2) + 1);
        strcat(result, str1);
        strcat(result, str2);
        result = addDoubleQuote(result);
        t->value.intValue = getStringIdex_insert(result);
        t->valueType = V_INT;
    } else if (isTypeOf(operand1, STR) && isTypeOf(operand2, INT)) {
        t->type = STR;
        char* str1 = removeDoubleQuote(stringTable[operand1->value.intValue - 1]);
        char str2[20];
        sprintf(str2, "%d", operand2->value.intValue);
        char* result = malloc(strlen(str1) + strlen(str2) + 1);
        strcat(result, str1);
        strcat(result, str2);
        result = addDoubleQuote(result);
        t->value.intValue = getStringIdex_insert(result);
        t->valueType = V_INT;
    } else if (isTypeOf(operand1, REAL) && isTypeOf(operand2, STR)) {
        t->type = STR;
        char str1[20];
        sprintf(str1, "%g", operand1->value.doubleValue);
        char* str2 = removeDoubleQuote(stringTable[operand2->value.intValue - 1]);
        char* result = malloc(strlen(str1) + strlen(str2) + 1);
        strcat(result, str1 + (((int)operand2->value.doubleValue) == 0));
        strcat(result, str2);
        result = addDoubleQuote(result);
        t->value.intValue = getStringIdex_insert(result);
        t->valueType = V_INT;
    } else if (isTypeOf(operand1, STR) && isTypeOf(operand2, REAL)) {
        t->type = STR;
        char* str1 = removeDoubleQuote(stringTable[operand1->value.intValue - 1]);
        char str2[20];
        sprintf(str2, "%g", operand2->value.doubleValue);
        char* result = malloc(strlen(str1) + strlen(str2) + 1);
        strcat(result, str1);
        strcat(result, str2 + (((int)operand2->value.doubleValue) == 0));
        result = addDoubleQuote(result);
        t->value.intValue = getStringIdex_insert(result);
        t->valueType = V_INT;
    }
    ParseNode* pn = malloc(sizeof(ParseNode));
    pn->current = t;
    return pn;
}

ParseNode* operateMinus(Token* operand1, Token* operand2) {
    Token* t = malloc(sizeof(Token));
    if (operand2 == NULL && isTypeOf(operand1, INT)) {
        t->type = INT;
        t->value.intValue = -1 * operand1->value.intValue;
        t->valueType = V_INT;
    } else if (operand2 == NULL && isTypeOf(operand1, REAL)) {
        t->type = REAL;
        t->value.doubleValue = -1 * operand1->value.doubleValue;
        t->valueType = V_REAL;
    } else if (isTypeOf(operand1, INT) && isTypeOf(operand2, INT)) {
        t->type = INT;
        t->value.intValue = operand1->value.intValue - operand2->value.intValue;
        t->valueType = V_INT;
    } else if (isTypeOf(operand1, INT) && isTypeOf(operand2, REAL)) {
        t->type = REAL;
        t->value.doubleValue = operand1->value.intValue - operand2->value.doubleValue;
        t->valueType = V_REAL;
    } else if (isTypeOf(operand1, REAL) && isTypeOf(operand2, INT)) {
        t->type = REAL;
        t->value.doubleValue = operand1->value.doubleValue - operand2->value.intValue;
        t->valueType = V_REAL;
    } else if (isTypeOf(operand1, REAL) && isTypeOf(operand2, REAL)) {
        t->type = REAL;
        t->value.doubleValue = operand1->value.doubleValue - operand2->value.doubleValue;
        t->valueType = V_REAL;
    }

    ParseNode* pn = malloc(sizeof(ParseNode));
    pn->current = t;
    return pn;
}

ParseNode* operateMultiply(Token* operand1, Token* operand2) {
    Token* t = malloc(sizeof(Token));
    if (isTypeOf(operand1, INT) && isTypeOf(operand2, INT)) {
        t->type = INT;
        t->value.intValue = operand1->value.intValue * operand2->value.intValue;
        t->valueType = V_INT;
    } else if (isTypeOf(operand1, INT) && isTypeOf(operand2, REAL)) {
        t->type = REAL;
        t->value.doubleValue = operand1->value.intValue * operand2->value.doubleValue;
        t->valueType = V_REAL;
    } else if (isTypeOf(operand1, REAL) && isTypeOf(operand2, INT)) {
        t->type = REAL;
        t->value.doubleValue = operand1->value.doubleValue * operand2->value.intValue;
        t->valueType = V_REAL;
    } else if (isTypeOf(operand1, REAL) && isTypeOf(operand2, REAL)) {
        t->type = REAL;
        t->value.doubleValue = operand1->value.doubleValue * operand2->value.doubleValue;
        t->valueType = V_REAL;
    } else if (isTypeOf(operand1, STR) && isTypeOf(operand2, INT)) {
        t->type = STR;
        char* s = repeatStr(stringTable[operand1->value.intValue - 1], operand2->value.intValue);
        t->value.intValue = getStringIdex_insert(s);
        t->valueType = V_INT;
    }
    ParseNode* pn = malloc(sizeof(ParseNode));
    pn->current = t;
    return pn;
}

ParseNode* operateDivide(Token* operand1, Token* operand2) {
    Token* t = malloc(sizeof(Token));
    if (isTypeOf(operand1, INT) && isTypeOf(operand2, INT)) {
        t->type = INT;
        t->value.intValue = operand1->value.intValue / operand2->value.intValue;
        t->valueType = V_INT;
    } else if (isTypeOf(operand1, INT) && isTypeOf(operand2, REAL)) {
        t->type = REAL;
        t->value.doubleValue = operand1->value.intValue / operand2->value.doubleValue;
        t->valueType = V_REAL;
    } else if (isTypeOf(operand1, REAL) && isTypeOf(operand2, INT)) {
        t->type = REAL;
        t->value.doubleValue = operand1->value.doubleValue / operand2->value.intValue;
        t->valueType = V_REAL;
    } else if (isTypeOf(operand1, REAL) && isTypeOf(operand2, REAL)) {
        t->type = REAL;
        t->value.doubleValue = operand1->value.doubleValue / operand2->value.doubleValue;
        t->valueType = V_REAL;
    } else if (isTypeOf(operand1, STR) && isTypeOf(operand2, STR)) {
        t->type = INT;
        t->value.intValue = divideStr(stringTable[operand1->value.intValue - 1], stringTable[operand2->value.intValue - 1]);
        t->valueType = V_INT;
    }
    ParseNode* pn = malloc(sizeof(ParseNode));
    pn->current = t;
    return pn;
}

ParseNode* operateAssign(Token* operand1, Token* operand2) {
    if (!isTypeOf(operand1, VARIABLE)) {
        syntax_error("assign");
    }

    SymbolEntry* se = &symbolTable[operand1->value.intValue - 1];
    if (isTypeOf(operand2, INT)) {
        se->token = (Token){INT, operand2->value.intValue, V_INT};
    } else if (isTypeOf(operand2, REAL)) {
        se->token = (Token){REAL, operand2->value.doubleValue, V_REAL};
    } else if (isTypeOf(operand2, STR)) {
        se->token = (Token){STR, operand2->value.intValue, V_INT};
    }
    ParseNode* pn = malloc(sizeof(ParseNode));
    pn->current = operand2;
    return pn;
}

ParseNode* operateSub(Token* str, Token* n1, Token* n2) {
    Token* t = malloc(sizeof(Token));

    t->type = STR;
    char* s = addDoubleQuote(substring(removeDoubleQuote(stringTable[str->value.intValue - 1]), n1->value.intValue, n2->value.intValue));
    t->value.intValue = getStringIdex_insert(s);
    t->valueType = V_INT;

    ParseNode* pn = malloc(sizeof(ParseNode));
    pn->current = t;
    return pn;
}

ParseNode* solveVariable(Token* variable) {
    Token* t;
    SymbolEntry* se = &symbolTable[variable->value.intValue - 1];
    if (isTypeOf(&se->token, INT) || isTypeOf(&se->token, REAL) || isTypeOf(&se->token, STR)) {
        t = &se->token;
    } else {
        t = variable;
    }
    ParseNode* pn = malloc(sizeof(ParseNode));
    pn->current = t;
    return pn;
}
