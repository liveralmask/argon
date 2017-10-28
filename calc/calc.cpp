#include <argon.h>

static ArgonParser g_parser;

static void* on_token( ssize_t type, const char* string, size_t length ){
  printf( "on_token type=%ld string=%s length=%ld\n", type, string, length );
  return NULL;
}

static void* on_syntax( ssize_t type, size_t count, va_list list ){
  printf( "on_syntax type=%ld(%c) count=%ld\n", type, isprint( type ) ? (char)type : ' ', count );
  return NULL;
}

int main( int argc, char** argv ){
  char src[ 256 ];
  for ( int argi = 1; argi < argc; ++argi ){
    snprintf( src, sizeof( src ), "arg%02d", argi );
    int result = argon_parse_string( &g_parser, src, on_token, on_syntax, argv[ argi ] );
    if ( 0 != result ) exit( result );
  }
  return 0;
}
