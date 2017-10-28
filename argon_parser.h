#ifndef __ARGON_PARSER_H__
#define __ARGON_PARSER_H__

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>

#include <parser.h>

typedef void* (*ArgonTokenCallback)( ssize_t type, const char* string, size_t length );
typedef void* (*ArgonSyntaxCallback)( ssize_t type, size_t count, va_list list );

typedef struct {
  const char*         src;
  size_t              line;
  ArgonTokenCallback  token_callback;
  ArgonSyntaxCallback syntax_callback;
} ArgonParser;

#ifdef __cplusplus
extern "C" {
#endif

int argon_parse_string( ArgonParser* parser, const char* src, ArgonTokenCallback  token_callback, ArgonSyntaxCallback syntax_callback, const char* string );
int argon_parse_file( ArgonParser* parser, const char* src, ArgonTokenCallback  token_callback, ArgonSyntaxCallback syntax_callback, FILE* file );

void* argon_node_new_token( ArgonParser* parser, ssize_t type, const char* string, size_t length );
void* argon_node_new_syntax( ArgonParser* parser, ssize_t type, size_t count, ... );

#ifdef __cplusplus
}
#endif

#endif
