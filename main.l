%{
#define OPERATOR    1
#define BRACKET     2
#define STRING      3
#define INTEGER     4
#define FLOAT       5
#define VARIABLE    6
#define BUILTIN_FUCTION 7
#define COMMA       8
#define PARSER_DIRECTIVE    9
%}
comment #.*
blank   [ \t\n]+
letter  [a-zA-Z]
digit   [0-9]
%%
{comment}   ;
{blank}     ;
"+"|"-"|"*"|"/"|"=" {return OPERATOR;}

"("|")" {return BRACKET;}

"," {return COMMA;}

sub {return BUILTIN_FUCTION;}

{digit}+    {return INTEGER;}

{digit}+"."{digit}* |
{digit}*"."{digit}+ {return FLOAT;}

{letter}({letter}|{digit})* {return VARIABLE;}

\"(\\([\nabfnrt\\'"]|ooo|xdd)|[^"\\\n])*\"  {return STRING;}

$ast|$symbol    {return PARSER_DIRECTIVE;}

%%
int main() {
    int token;
    while ((token=yylex()) != 0) {
        if (token == OPERATOR) {
            printf("operator\n");
        } else if (token == INTEGER) {
            printf("interger\n");
        } else if (token == FLOAT) {
            printf("float\n");
        } else if (token == VARIABLE) {
            printf("variable\n");
        } else if (token == STRING) {
            printf("string\n");
        } else if (token == BRACKET) {
            printf("bracket\n");
        } else if (token == COMMA) {
            printf("comma\n");
        } else if (token == BUILTIN_FUCTION) {
            printf("built in function\n");
        } else if (token == PARSER_DIRECTIVE) {
            printf("parser directive\n");
        } else {
            printf("else\n");
        }
    }
}