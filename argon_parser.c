#include <argon.h>

int yy_scan_string( const char* string );
int yyrestart( FILE* file );
int yyparse( ArgonParser* parser );
extern int yydebug;

int argon_parse_string( ArgonParser* parser, const char* src, ArgonTokenCallback  token_callback, ArgonSyntaxCallback syntax_callback, const char* string ){
  parser->src             = src;
  parser->line            = 1;
  parser->token_callback  = token_callback;
  parser->syntax_callback = syntax_callback;
  
  yy_scan_string( string );
  return yyparse( parser );
}

int argon_parse_file( ArgonParser* parser, const char* src, ArgonTokenCallback  token_callback, ArgonSyntaxCallback syntax_callback, FILE* file ){
  parser->src             = src;
  parser->line            = 1;
  parser->token_callback  = token_callback;
  parser->syntax_callback = syntax_callback;
  
  yyrestart( file );
  return yyparse( parser );
}

void* argon_node_new_token( ArgonParser* parser, ssize_t type, const char* string, size_t length ){
  return parser->token_callback( type, string, length );
}

void* argon_node_new_syntax( ArgonParser* parser, ssize_t type, size_t count, ... ){
  va_list list;
  va_start( list, count );
  void* node = parser->syntax_callback( type, count, list );
  va_end( list );
  return node;
}
