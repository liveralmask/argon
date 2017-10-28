#ifndef __ARGON_PARSER_H__
#define __ARGON_PARSER_H__

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>

#include <parser.h>

typedef void* (*ArgonNodeNewTokenCallback)( ssize_t type, const char* string, size_t length );
typedef void* (*ArgonNodeNewSyntaxCallback)( ssize_t type, size_t count, va_list list );

typedef struct {
  const char*                src;
  size_t                     line;
  ArgonNodeNewTokenCallback  node_new_token_callback;
  ArgonNodeNewSyntaxCallback node_new_syntax_callback;
} ArgonParser;

ARGON_EXTERN_C_BEGIN
int argon_parse_string( ArgonParser* parser, const char* src, ArgonNodeNewTokenCallback node_new_token_callback, ArgonNodeNewSyntaxCallback node_new_syntax_callback, const char* string );
int argon_parse_file( ArgonParser* parser, const char* src, ArgonNodeNewTokenCallback node_new_token_callback, ArgonNodeNewSyntaxCallback node_new_syntax_callback, FILE* file );

void* argon_node_new_token( ArgonParser* parser, ssize_t type, const char* string, size_t length );
void* argon_node_new_syntax( ArgonParser* parser, ssize_t type, size_t count, ... );
ARGON_EXTERN_C_END

#endif
