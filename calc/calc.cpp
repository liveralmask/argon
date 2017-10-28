#include <argon.h>

static ArgonParser g_parser;
static ArgonRunner g_runner;

static void* node_new_token( ssize_t type, const char* string, size_t length ){
//  printf( "  node_new_token type=%ld string=%s length=%ld\n", type, string, length );
  ArgonNode* node = argon_node_new( ARGON_NODE_TYPE_NIL );
  switch ( type ){
  case INTEGER:{
    node->type = ARGON_NODE_TYPE_DATA_INT;
    node->value.i = strtoll( string, NULL, 0 );
  }break;
  }
  return node;
}

static void* node_new_syntax( ssize_t type, size_t count, va_list list ){
//  printf( "  node_new_syntax type=%ld(%c) count=%ld\n", type, isprint( type ) ? (char)type : ' ', count );
  ArgonNode* node = NULL;
  switch ( type ){
  case 0:{
    g_runner.node = node = ARGON_NODE_ARG( list );
  }break;
  
  case -1:{
    node = ARGON_NODE_ARG( list );
    node->value.i *= -1;
  }break;
  
  case '+':{
    node = argon_node_new( ARGON_NODE_TYPE_OP_ADD );
    node->value.op.left  = ARGON_NODE_ARG( list );
    node->value.op.right = ARGON_NODE_ARG( list );
  }break;
  
  case '-':{
    node = argon_node_new( ARGON_NODE_TYPE_OP_SUB );
    node->value.op.left  = ARGON_NODE_ARG( list );
    node->value.op.right = ARGON_NODE_ARG( list );
  }break;
  
  case '*':{
    node = argon_node_new( ARGON_NODE_TYPE_OP_MUL );
    node->value.op.left  = ARGON_NODE_ARG( list );
    node->value.op.right = ARGON_NODE_ARG( list );
  }break;
  
  case '/':{
    node = argon_node_new( ARGON_NODE_TYPE_OP_DIV );
    node->value.op.left  = ARGON_NODE_ARG( list );
    node->value.op.right = ARGON_NODE_ARG( list );
  }break;
  
  case '%':{
    node = argon_node_new( ARGON_NODE_TYPE_OP_MOD );
    node->value.op.left  = ARGON_NODE_ARG( list );
    node->value.op.right = ARGON_NODE_ARG( list );
  }break;
  
  default:{
    fprintf( stderr, "Unsupported type: %ld(%c) # %s:%zd\n", type, isprint( type ) ? (char)type : ' ', g_parser.src, g_parser.line );
    exit( EXIT_FAILURE );
  }break;
  }
  return node;
}

int main( int argc, char** argv ){
  int result = 0;
  char src[ 256 ];
  for ( int argi = 1; argi < argc; ++argi ){
    g_runner.node = NULL;
    snprintf( src, sizeof( src ), "arg%d", argi );
    result = argon_parse_string( &g_parser, src, node_new_token, node_new_syntax, argv[ argi ] );
    if ( 0 == result ) result = argon_run( &g_runner );
    if ( 0 != result ) break;
  }
  return result;
}
