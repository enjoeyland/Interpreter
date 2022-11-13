#include "token_type.h"

#include <stdio.h>
#include <string.h>

char* tokenTypeName[] = {"", "variable", "int", "real", "string", ",", "\\n", "split", "+", "-", "*", "/", "=", "(", ")"};

Token failToken = {-1, -1, NONE};
int isEqual(Token t1, Token t2) {
    return memcmp(&t1, &t2, sizeof(Token)) == 0;
}
int isFailToken(Token t) {
    return isEqual(t, failToken);
}
int isNewlineToken(Token t) {
    return isEqual(t, (Token){NEW_LINE, 0, NONE});
}