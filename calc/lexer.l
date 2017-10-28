%{
#include <argon.h>

#define YY_DECL int yylex( YYSTYPE* data, ArgonParser* parser )
%}

%option noyywrap
%option nounput
%option noinput
%option yylineno

%%
0|[1-9][0-9]*|0x[0-9a-fA-F]+ {
  parser->line = yylineno;
  data->node = argon_node_new_token( parser, INTEGER, yytext, (size_t)yyleng );
  return INTEGER;
}

\+ {
  parser->line = yylineno;
  return '+';
}

\- {
  parser->line = yylineno;
  return '-';
}

\* {
  parser->line = yylineno;
  return '*';
}

\/ {
  parser->line = yylineno;
  return '/';
}

\% {
  parser->line = yylineno;
  return '%';
}

\( {
  parser->line = yylineno;
  return '(';
}

\) {
  parser->line = yylineno;
  return ')';
}

\<\< {
  parser->line = yylineno;
  return L_SHIFT;
}

\>\> {
  parser->line = yylineno;
  return R_SHIFT;
}

[ \t] {
  
}

. {
  char c = yytext[ 0 ];
  fprintf( stderr, "token error, %c(0x%02X) %s:%zd\n", ( 0 == isprint( c ) ) ? ' ' : c, (unsigned char)c, parser->src, parser->line );
  exit( EXIT_FAILURE );
}
%%
