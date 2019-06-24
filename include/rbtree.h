#ifndef RBTREE
#define RBTREE

#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "structs.h"
#define BLACK 'B'
#define RED 'R'

#define FOUND 0
#define INSERTED 1

typedef struct s_node{
  scanned_path *file;
  char color;
  struct s_node *left;
  struct s_node *right;
  struct s_node *parent;
} treenode;

void init_rbtree(void);
scanned_path *add_rbtree(scanned_path *file);
void inorder_visit(void);
scanned_path *rbnext(void);


#endif
