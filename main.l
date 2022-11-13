%option header-file="lex.yy.h"

%{
#include "token_type.h"
%}

comment #.*
blank   [ \t]+
letter  [a-zA-Z]
digit   [0-9]
%%
{comment}   ;
{blank}     ;
[+\-*/=] {return C_OPERATOR;}

[()] {return C_BRACKET;}

, {return C_COMMA;}
\n {return C_NEW_LINE;}
sub {return C_BUILTIN_FUCTION;}

{digit}+    {return C_INT;}

{digit}+"."{digit}* |
{digit}*"."{digit}+ {return C_REAL;}

{letter}({letter}|{digit})* {return C_VARIABLE;}

\"(\\([\nabfnrt\\'"]|ooo|xdd)|[^"\\\n])*\"  {return C_STR;}

$ast|$symbol    {return C_PARSER_DIRECTIVE;}
. {return C_FAIL;}
%%
