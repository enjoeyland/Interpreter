%option header-file="lex.yy.h"

%{
#include "token_type.h"
%}

%{
extern int char_count;
%}
comment #.*
blank   [ \t]+
letter  [a-zA-Z]
digit   [0-9]
%%
{comment}   ;
{blank}     {char_count+=yyleng;};
[+\-*/=] {char_count+=yyleng; return C_OPERATOR;}

[()] {char_count+=yyleng; return C_BRACKET;}

, {char_count+=yyleng; return C_COMMA;}
\n {char_count+=yyleng; return C_NEW_LINE;}
sub {char_count+=yyleng; return C_BUILTIN_FUCTION;}

{digit}+    {char_count+=yyleng; return C_INT;}

{digit}+"."{digit}* |
{digit}*"."{digit}+ {char_count+=yyleng; return C_REAL;}

{letter}({letter}|{digit})* {char_count+=yyleng; return C_VARIABLE;}

\"(\\([\nabfnrt\\'"]|ooo|xdd)|[^"\\\n])*\"  {char_count+=yyleng; return C_STR;}

$ast|$symbol    {char_count+=yyleng; return C_PARSER_DIRECTIVE;}
. {char_count+=yyleng; return C_FAIL;}
%%
