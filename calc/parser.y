%{
#include <argon.h>

#define YYERROR_VERBOSE 1
#define YYDEBUG 1
%}

%union {
  void* node;
}

%pure-parser
%parse-param {ArgonParser* parser}
%lex-param {ArgonParser* parser}

%{
int yylex( YYSTYPE* data, ArgonParser* parser );
static void yyerror( ArgonParser* parser, const char* string ){
  fprintf( stderr, "%s %s:%zd\n", string, parser->src, parser->line );
}
%}

%token INTEGER

%type <node> INTEGER program lt_integer expr
%left L_SHIFT R_SHIFT
%left '+' '-' '%'
%left '*' '/'

%%
program : expr { $$ = argon_node_new_syntax( parser, 0, 1, $1 ); }
lt_integer : INTEGER
           | '+' INTEGER { $$ = argon_node_new_syntax( parser, 1, 1, $2 ); }
           | '-' INTEGER { $$ = argon_node_new_syntax( parser, -1, 1, $2 ); }
expr : lt_integer
     | expr '*' expr { $$ = argon_node_new_syntax( parser, '*', 2, $1, $3 ); }
     | expr '/' expr { $$ = argon_node_new_syntax( parser, '/', 2, $1, $3 ); }
     | expr '+' expr { $$ = argon_node_new_syntax( parser, '+', 2, $1, $3 ); }
     | expr '-' expr { $$ = argon_node_new_syntax( parser, '-', 2, $1, $3 ); }
     | expr '%' expr { $$ = argon_node_new_syntax( parser, '%', 2, $1, $3 ); }
     | expr L_SHIFT expr { $$ = argon_node_new_syntax( parser, L_SHIFT, 2, $1, $3 ); }
     | expr R_SHIFT expr { $$ = argon_node_new_syntax( parser, R_SHIFT, 2, $1, $3 ); }
     | '(' expr ')' { $$ = argon_node_new_syntax( parser, 1, 1, $2 ); }
%%
