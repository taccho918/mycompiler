%{
#include "parse.tab.h"
%}
%%
"define" {return DEFINE;}
"array" {return ARRAY;}
"while" {return WHILE;}
"if" {return IF;}
"else" {return ELSE;}
"for" {return FOR;}
"==" {return EQUAL;}
"++" {return INCREMENT;}
"--" {return DECREMENT;}
"<=" {return LESSEQUAL;}
">=" {return GREATEREQUAL;}
"+" {return PLUS;}
"-" {return MINUS;}
"*" {return MUL;}
"/" {return DIV;}
"=" {return ASSIGN;}
"<" {return LESS;}
">" {return GREATER;}

[a-zA-Z][a-zA-Z0-9]* {yylval.id = strdup(yytext); return IDENTIFIER;}
[0-9]+ {yylval.num = atoi(yytext); return NUM;}
[ \n\t] 

. return yytext[0];
%%