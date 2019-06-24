#include "rbtree.h"

struct s_node s_nullnode;
treenode *nullnode = &s_nullnode;
treenode *root = NULL;


static treenode *newtreenode(scanned_path *file);
static void rotateleft(treenode **root, treenode *x);
static void rotateright(treenode **root, treenode *x);
static scanned_path *rbinsert(treenode **root, scanned_path *file);
static void insertfixup(treenode **root, treenode *newnode);

typedef struct tree_descriptor{
  treenode *root;
  long int size;
}tree_descriptor;

static tree_descriptor this;
static tree_descriptor *thistree = &this;

void init_rbtree(void)
{
  thistree->root = nullnode;
  thistree->size = 0;
}

scanned_path *add_rbtree(scanned_path *file)
{
  return rbinsert(&thistree->root, file);

}



static treenode *newtreenode(scanned_path *file)
{
  treenode *temp = (treenode*) malloc(sizeof(treenode));
  temp->file = file;
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

static scanned_path *rbinsert(treenode **root, scanned_path *file)
{
  treenode *newnode = newtreenode(file);
  treenode *y = nullnode;
  treenode *x = root[0];
  int compare;
  printf("rbinsert: inserting %s\n", newnode->file->path);
  while(x != nullnode){
    y = x;
    compare = strcmp(newnode->file->path, x->file->path);
    // printf("%s %d %s\n", newnode->file->path, compare, x->file->path);
    if(compare < 0)
      x = x->left;
    else if(compare > 0)
      x = x->right;
    else
      return x->file;
  }
  thistree->size++;
  newnode->parent = y;
  if(y == nullnode)
    *root = newnode;
  else if(strcmp(newnode->file->path, y->file->path) < 0)
    y->left = newnode;
  else
    y->right = newnode;

  newnode->left = nullnode;
  newnode->right = nullnode;
  newnode->color = RED;

  // printf("rbinsert: inserted\n");
  insertfixup(root, newnode);
  return newnode->file;
}

static void insertfixup(treenode **root, treenode *newnode)
{
  treenode *temp;

  while((newnode != *root) && (newnode->parent->color == RED)){
    // printf("insertfixup: fixing %ld\n", newnode->inode);
    if(newnode->parent
      == newnode->parent->parent->left){
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
        if(newnode == newnode->parent->right){
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
        if(newnode == newnode->parent->left){
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

static int count;
static scanned_path **pathlist;

static void inorder(treenode *root);

void inorder_visit(void)
{
  if(thistree->root == nullnode){
    return;
  }
  printf("SIZE: %ld\n",thistree->size);
  pathlist = (scanned_path**) malloc(sizeof(scanned_path) * thistree->size);
  count = 0;
  inorder(thistree->root);
  count = 0;
  return;
  int i = thistree->size;
  // while(--i >= 0){
  //   printf("INORDER: %s\n", pathlist[i]->path);
  // }
}

scanned_path *rbnext(void)
{
  if(count == thistree->size)
    return NULL;

  return pathlist[count++];
}



void inorder(treenode *root)
{
  if(root->left != nullnode){
    inorder(root->left);
  }
  pathlist[count] = root->file;
  count++;
  if(root->right != nullnode){
    inorder(root->right);
  }
}
