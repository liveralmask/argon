#include <argon.h>

int argon_run( ArgonRunner* runner ){
  argon_node_dump( runner->node, 0 );
  ArgonNode* result = argon_node_eval( runner->node );
  argon_node_dump( result, 0 );
  if ( result != runner->node ) argon_node_delete( result );
  argon_node_delete( runner->node );
  return 0;
}

ArgonNode* argon_node_new( ArgonNodeType type ){
  ArgonNode* node = (ArgonNode*)malloc( sizeof( ArgonNode ) );
  node->type = type;
  return node;
}

void argon_node_delete( ArgonNode* node ){
  if ( 0 != ( ARGON_NODE_TYPE_OP & node->type ) ){
    argon_node_delete( node->value.op.left );
    argon_node_delete( node->value.op.right );
  }
  free( node );
}

void argon_node_dump( ArgonNode* node, size_t indent ){
  for ( size_t i = 0; i < indent; ++i ) printf( "  " );
  
  switch ( node->type ){
  case ARGON_NODE_TYPE_DATA_INT:{
    printf( "%lld|%llu(0x%llX)\n", node->value.i, node->value.u, node->value.u );
  }break;
  
  case ARGON_NODE_TYPE_DATA_NAN:{
    printf( "NaN\n" );
  }break;
  
  case ARGON_NODE_TYPE_OP_ADD:{
    printf( "+\n" );
    argon_node_dump( node->value.op.left, indent + 1 );
    argon_node_dump( node->value.op.right, indent + 1 );
  }break;
  
  case ARGON_NODE_TYPE_OP_SUB:{
    printf( "-\n" );
    argon_node_dump( node->value.op.left, indent + 1 );
    argon_node_dump( node->value.op.right, indent + 1 );
  }break;
  
  case ARGON_NODE_TYPE_OP_MUL:{
    printf( "*\n" );
    argon_node_dump( node->value.op.left, indent + 1 );
    argon_node_dump( node->value.op.right, indent + 1 );
  }break;
  
  case ARGON_NODE_TYPE_OP_DIV:{
    printf( "/\n" );
    argon_node_dump( node->value.op.left, indent + 1 );
    argon_node_dump( node->value.op.right, indent + 1 );
  }break;
  
  case ARGON_NODE_TYPE_OP_MOD:{
    printf( "%%\n" );
    argon_node_dump( node->value.op.left, indent + 1 );
    argon_node_dump( node->value.op.right, indent + 1 );
  }break;
  
  default:{
    fprintf( stderr, "Unsupported type: %d(0x%X) # %s:%zd\n", (int)node->type, (int)node->type, __FILE__, __LINE__ );
    exit( EXIT_FAILURE );
  }break;
  }
}

ArgonNode* argon_node_eval( ArgonNode* node ){
  if ( 0 != ( ARGON_NODE_TYPE_DATA & node->type ) ) return node;
  
  ArgonNode* result = NULL;
  switch ( node->type ){
  case ARGON_NODE_TYPE_OP_ADD:{
    ArgonNode* left = argon_node_eval( node->value.op.left );
    ArgonNode* right = argon_node_eval( node->value.op.right );
    result = argon_node_new( ARGON_NODE_TYPE_DATA_INT );
    result->value.i = left->value.i + right->value.i;
    if ( left != node->value.op.left ) argon_node_delete( left );
    if ( right != node->value.op.right ) argon_node_delete( right );
  }break;
  
  case ARGON_NODE_TYPE_OP_SUB:{
    ArgonNode* left = argon_node_eval( node->value.op.left );
    ArgonNode* right = argon_node_eval( node->value.op.right );
    result = argon_node_new( ARGON_NODE_TYPE_DATA_INT );
    result->value.i = left->value.i - right->value.i;
    if ( left != node->value.op.left ) argon_node_delete( left );
    if ( right != node->value.op.right ) argon_node_delete( right );
  }break;
  
  case ARGON_NODE_TYPE_OP_MUL:{
    ArgonNode* left = argon_node_eval( node->value.op.left );
    ArgonNode* right = argon_node_eval( node->value.op.right );
    result = argon_node_new( ARGON_NODE_TYPE_DATA_INT );
    result->value.i = left->value.i * right->value.i;
    if ( left != node->value.op.left ) argon_node_delete( left );
    if ( right != node->value.op.right ) argon_node_delete( right );
  }break;
  
  case ARGON_NODE_TYPE_OP_DIV:{
    ArgonNode* left = argon_node_eval( node->value.op.left );
    ArgonNode* right = argon_node_eval( node->value.op.right );
    if ( 0 == right->value.i ){
      result = argon_node_new( ARGON_NODE_TYPE_DATA_NAN );
    }else{
      result = argon_node_new( ARGON_NODE_TYPE_DATA_INT );
      result->value.i = left->value.i / right->value.i;
    }
    if ( left != node->value.op.left ) argon_node_delete( left );
    if ( right != node->value.op.right ) argon_node_delete( right );
  }break;
  
  case ARGON_NODE_TYPE_OP_MOD:{
    ArgonNode* left = argon_node_eval( node->value.op.left );
    ArgonNode* right = argon_node_eval( node->value.op.right );
    if ( 0 == right->value.i ){
      result = argon_node_new( ARGON_NODE_TYPE_DATA_NAN );
    }else{
      result = argon_node_new( ARGON_NODE_TYPE_DATA_INT );
      result->value.i = left->value.i % right->value.i;
    }
    if ( left != node->value.op.left ) argon_node_delete( left );
    if ( right != node->value.op.right ) argon_node_delete( right );
  }break;
  
  default:{
    fprintf( stderr, "Unsupported type: %d(0x%X) # %s:%zd\n", (int)node->type, (int)node->type, __FILE__, __LINE__ );
    exit( EXIT_FAILURE );
  }break;
  }
  return result;
}
