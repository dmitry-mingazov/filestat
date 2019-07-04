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
  treenode_data data;
  char color;
  struct s_node *left;
  struct s_node *right;
  struct s_node *parent;
} treenode;

typedef struct tree_descriptor{
  treenode *root;
  long int size;
}tree_descriptor;

tree_descriptor *init_rbtree(void);
int contains_rbtree(tree_descriptor *tree, treenode_data *data);
treenode_data *add_rbtree(tree_descriptor *tree, treenode_data **data);
scanned_path **inorder_visit(tree_descriptor *tree, long int *size);
// scanned_path *rbnext(tree_descriptor *tree)


#endif
