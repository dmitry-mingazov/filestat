#include "rbtree.h"

struct s_node s_nullnode;
treenode *nullnode = &s_nullnode;
treenode *root = NULL;

static treenode *newtreenode(ino_t inode);
static void rotateleft(treenode **root, treenode *x);
static void rotateright(treenode **root, treenode *x);
static int rbinsert(treenode **root, ino_t inode);
static void insertfixup(treenode **root, treenode *newnode);

void init_rbtree(void)
{
    root = nullnode;
}

int add_rbtree(ino_t inode)
{
  return rbinsert(&root, inode);
}



static treenode *newtreenode(ino_t inode)
{
  treenode *temp = (treenode*) malloc(sizeof(treenode));
  temp->inode = inode;
  temp->color = RED;
  temp->left = NULL;
  temp->right = NULL;
  temp->parent = NULL;

  return temp;
}

static void rotateleft(treenode **root, treenode *x)
{
  treenode *y = x->right;
  // printf("-rotating left\n");
  x->right = y->left;
  if(y->left != nullnode)
    y->left->parent = x;
  y->parent = x->parent;
  if(x->parent == nullnode)
    *root = y;
  else if(x == x->parent->left)
    x->parent->left = y;
  else
    x->parent->right = y;
  y->left = x;
  x->parent = y;
}

static void rotateright(treenode **root, treenode *x)
{
  treenode *y = x->left;
  // printf("-rotating right\n");
  x->left = y->right;
  if(y->right != nullnode)
    y->right->parent = x;
  y->parent = x->parent;
  if(x->parent == nullnode)
    *root = y;
  else if(x == x->parent->right)
    x->parent->right = y;
  else
    x->parent->left = y;
  y->right = x;
  x->parent = y;
}

static int rbinsert(treenode **root, ino_t inode)
{
  treenode *newnode = newtreenode(inode);
  treenode *y = nullnode;
  treenode *x = *root;
  // printf("rbinsert: inserting %ld\n", inode);
  while(x != nullnode){
    y = x;
    if(newnode->inode < x->inode)
      x = x->left;
    else if(newnode->inode > x->inode)
      x = x->right;
    else
      return FOUND;
  }

  newnode->parent = y;
  if(y == nullnode)
    *root = newnode;
  else if(newnode->inode < y->inode)
    y->left = newnode;
  else
    y->right = newnode;

  newnode->left = nullnode;
  newnode->right = nullnode;
  newnode->color = RED;

  // printf("rbinsert: inserted\n");
  insertfixup(root, newnode);
  return INSERTED;
}

static void insertfixup(treenode **root, treenode *newnode)
{
  treenode *temp;

  while((newnode != *root) && (newnode->parent->color == RED)){
    // printf("insertfixup: fixing %ld\n", newnode->inode);
    if(newnode->parent->inode
      == newnode->parent->parent->left->inode){
      //if newnode's parent is a left child, temp is right uncle
      temp = newnode->parent->parent->right;
      if(temp->color == RED){
        //change colors
        newnode->parent->color = BLACK;
        temp->color = BLACK;
        newnode->parent->parent->color = RED;
        //move newnode up on the tree
        newnode = newnode->parent->parent;
      }
      else{
        //temp is a black node
        if(newnode->inode == newnode->parent->right->inode){
          newnode = newnode->parent;
          rotateleft(root, newnode);
        }
        newnode->parent->color = BLACK;
        newnode->parent->parent->color = RED;
        rotateright(root, newnode->parent->parent);
      }
    }
    else{
      // printf("insertfixup: newnode's parent is right child\n");
      //if newnode's parent is a right child, temp is left uncle
      temp = newnode->parent->parent->left;
      if(temp->color == RED){
        //change colors
        newnode->parent->color = BLACK;
        temp->color = BLACK;
        newnode->parent->parent->color = RED;
        //move newnode up on the tree
        newnode = newnode->parent->parent;
      }
      else{
        //temp is a black node
        if(newnode->inode == newnode->parent->left->inode){
          newnode = newnode->parent;
          rotateright(root, newnode);
        }
        newnode->parent->color = BLACK;
        newnode->parent->parent->color = RED;
        rotateleft(root, newnode->parent->parent);
      }
    }
  }
  root[0]->color = BLACK;
}
