#include "rbtree.h"

struct s_node s_nullnode;
treenode *nullnode = &s_nullnode;
treenode *root = NULL;


static treenode *newtreenode(treenode_data *data);
static void rotateleft(treenode **root, treenode *x);
static void rotateright(treenode **root, treenode *x);
static void rbinsert(tree_descriptor *tree, treenode_data **data);
static void insertfixup(treenode **root, treenode *newnode);
static int treenode_data_compare(treenode_data *x, treenode_data *y);
static void treenode_data_free(treenode_data *data);


// static tree_descriptor this;
// static tree_descriptor *thistree = &this;

tree_descriptor *init_rbtree(void)
{
  tree_descriptor *tree = (tree_descriptor*) malloc(sizeof(tree_descriptor));
  tree->root = nullnode;
  tree->size = 0;
  return tree;
}

void add_rbtree(tree_descriptor *tree, treenode_data **data)
{
  rbinsert(tree, data);
}

treenode_data *get_data_rbtree(tree_descriptor *tree, treenode_data *data)
{
  treenode *x = tree->root;
  int compare;
  while(x != nullnode){
    compare = treenode_data_compare(data, x->data);
    if(compare < 0)
      x = x->left;
    else if(compare > 0)
      x = x->right;
    else
      return x->data;
  }
  return NULL;
}

static treenode *newtreenode(treenode_data *data)
{
  treenode *temp = (treenode*) malloc(sizeof(treenode));
  temp->data = data;
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

static void rbinsert(tree_descriptor *tree, treenode_data **data)
{
  treenode *newnode = newtreenode(*data);
  treenode *y = nullnode;
  treenode *x;
  x = tree->root;
  int compare;
  // printf("rbinsert: inserting %s\n", newnode->file->path);
  while(x != nullnode){
    y = x;
    compare = treenode_data_compare(newnode->data, x->data);
    // printf("%s %d %s\n", newnode->file->path, compare, x->file->path);
    if(compare < 0)
      x = x->left;
    else if(compare > 0)
      x = x->right;
    else{
      treenode_data_free(*data);
      free(newnode);
      *data = x->data;
      return;
    }

  }
  tree->size++;
  newnode->parent = y;
  if(y == nullnode)
    tree->root = newnode;
  else if(compare < 0)
    y->left = newnode;
  else
    y->right = newnode;

  newnode->left = nullnode;
  newnode->right = nullnode;

  // printf("rbinsert: inserted\n");
  insertfixup(&tree->root, newnode);
}

static void insertfixup(treenode **root, treenode *newnode)
{
  treenode *temp;

  while((newnode != *root) && (newnode->parent->color == RED)){
    // printf("insertfixup: fixing %ld\n", newnode->inode);
    if(newnode->parent == newnode->parent->parent->left){
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

int treenode_data_compare(treenode_data *x, treenode_data *y)
{
  if(x->type != y->type){
    fprintf(stderr, "rbtree: Fatal error, different types of node data found\n");
    exit(1);
  }

  switch(x->type){
    case T_SCANNED_PATH:
      return strcmp(x->data.file->path, y->data.file->path);
    case T_INODE:
      if(x->data.inode > y->data.inode){
        return 1;
      }else if(x->data.inode < y->data.inode){
        return -1;
      }else{
        return 0;
      };
    default:
    fprintf(stderr, "rbtree: Fatal error, unknown type of data\n");
    exit(1);
  }
}

treenode_data *filepath_to_treenode_data(char *path)
{
  scanned_path *sp_empty = (scanned_path*) malloc(sizeof(scanned_path));
  //allocate memory to copy the path
  sp_empty->path = (char*) malloc(sizeof(char) * strlen(path) + 1);
  strcpy(sp_empty->path, path);
  //initialize head and tail to NULL
  sp_empty->head = NULL;
  sp_empty->tail = NULL;
  //create and allocate memory for new treenode_data
  treenode_data *pt_data = (treenode_data*) malloc(sizeof(treenode_data));
  //set its type to scanned_path
  pt_data->type = T_SCANNED_PATH;
  //create new union data
  u_data data;
  //copy new scanned_path struct pointer into union
  data.file = sp_empty;
  //copy union into treenode_data
  pt_data->data = data;

  return pt_data;
}

void treenode_data_free(treenode_data *data)
{
  if(data == NULL)
    return;

  switch(data->type){
    case T_SCANNED_PATH:
      free(data->data.file->path);
      free(data);
      break;
    case T_INODE:
      free(data);
      break;
    default:
    fprintf(stderr, "rbtree: Fatal error, unknown type of data\n");
    exit(1);
  }
}

static long int count;

static void inorder(treenode *root, scanned_path **pathlist);

scanned_path **inorder_visit(tree_descriptor *tree, long int *size)
{
  if(tree->root == nullnode){
    return NULL;
  }
  // printf("SIZE: %ld\n",thistree->size);
  scanned_path **pathlist = (scanned_path**) malloc(sizeof(scanned_path) * tree->size);
  count = 0;
  inorder(tree->root, pathlist);
  *size = count;
  count = 0;
  return pathlist;
  // int i = thistree->size;
  // while(--i >= 0){
  //   printf("INORDER: %s\n", pathlist[i]->path);
  // }
}

// scanned_path *rbnext(void)
// {
//   if(count == thistree->size)
//     return NULL;
//
//   return pathlist[count++];
// }



void inorder(treenode *root, scanned_path **pathlist)
{
  if(root->left != nullnode){
    inorder(root->left, pathlist);
  }
  pathlist[count] = root->data->data.file;
  count++;
  if(root->right != nullnode){
    inorder(root->right, pathlist);
  }
}
