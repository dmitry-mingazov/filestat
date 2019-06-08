#ifndef RBTREE
#define RBTREE

#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>

#define BLACK 'B'
#define RED 'R'

#define FOUND 0
#define INSERTED 1

typedef struct s_node{
  ino_t inode;
  char color;
  struct s_node *left;
  struct s_node *right;
  struct s_node *parent;
} treenode;

void init_rbtree(void);
int add_rbtree(ino_t inode);


#endif
