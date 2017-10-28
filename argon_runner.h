#ifndef __ARGON_RUNNER_H__
#define __ARGON_RUNNER_H__

#include <argon.h>

#define ARGON_NODE_ARG( _list ) (ArgonNode*)va_arg( _list, void* )

typedef enum {
  ARGON_NODE_TYPE_NIL       = 0,
  
  ARGON_NODE_TYPE_DATA      = 0x8000,
  ARGON_NODE_TYPE_DATA_BOOL = ARGON_NODE_TYPE_DATA | 1,
  ARGON_NODE_TYPE_DATA_INT  = ARGON_NODE_TYPE_DATA | 2,
  ARGON_NODE_TYPE_DATA_FLT  = ARGON_NODE_TYPE_DATA | 3,
  ARGON_NODE_TYPE_DATA_NAN  = ARGON_NODE_TYPE_DATA | 4,
  ARGON_NODE_TYPE_DATA_STR  = ARGON_NODE_TYPE_DATA | 5,
  
  ARGON_NODE_TYPE_OP        = 0x4000,
  ARGON_NODE_TYPE_OP_ADD    = ARGON_NODE_TYPE_OP | 1,
  ARGON_NODE_TYPE_OP_SUB    = ARGON_NODE_TYPE_OP | 2,
  ARGON_NODE_TYPE_OP_MUL    = ARGON_NODE_TYPE_OP | 3,
  ARGON_NODE_TYPE_OP_DIV    = ARGON_NODE_TYPE_OP | 4,
  ARGON_NODE_TYPE_OP_MOD    = ARGON_NODE_TYPE_OP | 5,
} ArgonNodeType;

typedef struct _ArgonNode {
  ArgonNodeType type;
  union {
    bool               b;
    long long          i;
    unsigned long long u;
    float              f;
    double             d;
    struct {
      char*  p;
      size_t len;
    } s;
    
    struct {
      struct _ArgonNode* left;
      struct _ArgonNode* right;
    } op;
  } value;
} ArgonNode;

typedef struct {
  ArgonNode* node;
} ArgonRunner;

ARGON_EXTERN_C_BEGIN
int argon_run( ArgonRunner* runner );

ArgonNode* argon_node_new( ArgonNodeType type );
void argon_node_delete( ArgonNode* node );

void argon_node_dump( ArgonNode* node, size_t indent );
ArgonNode* argon_node_eval( ArgonNode* node );
ARGON_EXTERN_C_END

#endif
