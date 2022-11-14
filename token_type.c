#include "token_type.h"

#include <string.h>

char* tokenTypeName[] = {"", "variable", "int", "real", "string", ",", "\\n", "sub", "+", "-", "*", "/", "=", "(", ")"};

Token failToken = {FAIL, -1, NONE};
int isEqual(Token t1, Token t2) {
    return memcmp(&t1, &t2, sizeof(Token)) == 0;
}
int isFailToken(Token t) {
    return isEqual(t, failToken);
}
int isEOFToken(Token t) {
    return isEqual(t, (Token){T_EOF, 0, NONE});
}
int isNewlineToken(Token t) {
    return isEqual(t, (Token){NEW_LINE, 0, NONE});
}

int isTypeOf(Token* token, TokenType token_type) {
    if (!token) return 0;
    return token->type == token_type;
}