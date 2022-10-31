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
[+\-*/=] {return OPERATOR;}

[()] {return BRACKET;}

, {return COMMA;}
\n {return NEW_LINE;}
sub {return BUILTIN_FUCTION;}

{digit}+    {return INTEGER;}

{digit}+"."{digit}* |
{digit}*"."{digit}+ {return FLOAT;}

{letter}({letter}|{digit})* {return VARIABLE;}

\"(\\([\nabfnrt\\'"]|ooo|xdd)|[^"\\\n])*\"  {return STRING;}

$ast|$symbol    {return PARSER_DIRECTIVE;}

%%
