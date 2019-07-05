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
  treenode_data *data;
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
treenode_data *get_data_rbtree(tree_descriptor *tree, treenode_data *data);
void add_rbtree(tree_descriptor *tree, treenode_data **data);
treenode_data *filepath_to_treenode_data(char *path);
treenode_data *inode_to_treenode_data(ino_t inode);
scanned_path **path_inorder(tree_descriptor *tree, long int *size);
// scanned_path *rbnext(tree_descriptor *tree)


#endif
