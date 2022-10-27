%{
#include "token_type.h"
%}
comment #.*
blank   [ \t\n]+
letter  [a-zA-Z]
digit   [0-9]
%%
{comment}   ;
{blank}     ;
[+_*/=] {return OPERATOR;}

[()] {return BRACKET;}

, {return COMMA;}

sub {return BUILTIN_FUCTION;}

{digit}+    {return INTEGER;}

{digit}+"."{digit}* |
{digit}*"."{digit}+ {return FLOAT;}

{letter}({letter}|{digit})* {return VARIABLE;}

\"(\\([\nabfnrt\\'"]|ooo|xdd)|[^"\\\n])*\"  {return STRING;}

$ast|$symbol    {return PARSER_DIRECTIVE;}

%%
