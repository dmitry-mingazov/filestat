/*************************************************************************
                           filestat
            UNICAM - Scuola di scienze e tecnologie
                   Facolta' di informatica

            Corso di Sistemi Operativi Laboratorio

  Copyright (C) 2019  Dmitry Mingazov, Beatrice Pucci Sisti

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.                                *
*************************************************************************/

#include "filestat.h"

//program configuration
static filestat_configuration fsconf;
//descriptor for tree of scanned_path
static tree_descriptor *tree;
//descriptor for tree of inode
static tree_descriptor *inode_tree;

int main(int argc, char **argv)
{
  //get options from command line
  getOptions(argc, argv, &fsconf);

  //input arguments for this execution
  input_file_argument *input_args = fsconf.input_args;
  //pointer to previous input arguments, neeeded to deallocate memory
  input_file_argument *prev = input_args;

  //initialize both trees
  tree = init_rbtree();
  inode_tree = init_rbtree();

  //if stat and report options are passed, initialize their funciontality
  if(HASOPT(fsconf.hasopt, STAT))
    init_stats();
  if(HASOPT(fsconf.hasopt, REPORT))
    init_report();

  //read output file if it exists, and save data on tree
  readOutputFile(fsconf.output_file, tree);
  //if history option is passed
  if(HASOPT(fsconf.hasopt, HISTORY)){
    //create a new treenode_data from the passed filepath
    treenode_data *history_data = filepath_to_treenode_data(fsconf.history);
    history_data = get_data_rbtree(tree, history_data);
    //if get_data_rbtree returns NULL, passed filepath don't exists
    if(history_data != NULL){
      print_history_of_file(history_data->data.file);
    }else{
      fprintf(stderr, "filestat: no history found for %s\n", fsconf.history);
    }
  }

  long int treesize = 0;
  //if noscan option is passed, don't execute filestat
  if(!(HASOPT(fsconf.hasopt, NOSCAN))){
    //execute filestat for every input argument
    while(input_args != NULL){
      filestat(input_args);
      prev = input_args;
      //move to next argument
      input_args = input_args->next;
      //deallocate memory of used arguments
      free(prev->path);
      free(prev);
    }
  }
  //ordered list of paths contained by tree
  scanned_path **pathlist = path_inorder(tree, &treesize);
  //if noscan option is passed, print pre-existing file info
  if(HASOPT(fsconf.hasopt, NOSCAN)){
    print_output_file(pathlist, treesize);
  }else{
    //else write to output file
    writeOutputFile(fsconf.output_file, pathlist, treesize);
  }

  //end and print stat and report info
  if(HASOPT(fsconf.hasopt, STAT))
    print_program_stats(end_stats());
  if(HASOPT(fsconf.hasopt, REPORT))
    print_program_report(end_report());
}
/*
  Core function, it take one argument per execution and analyze the
  file passed
  According to execution options, it slighty changes its behaviour
*/
void filestat(input_file_argument *input_args)
{
  struct stat stbuf;
  file_info *fsbuf = (file_info*) malloc(sizeof(file_info));
  if(fsbuf == NULL){
    fprintf(stderr, "%s", MALLOC_ERROR);
    exit(1);
  }
  //if it's necessary to describe file linked by links
  if(HASOPT(input_args->options, FOLLOW_LINK)){
    //use stat
    if(stat(input_args->path, &stbuf) == -1){
      fprintf(stderr, "filestat: can't access to %s\n", input_args->path);
      return;
    }
  }else{
    //else use lstat
    if(lstat(input_args->path, &stbuf) == -1){
      fprintf(stderr, "filestat: can't access to %s\n", input_args->path);
      return;
    }
  }
  //conserve only necessary data
  *fsbuf = statcpy(&stbuf);

  //initialize treenode_data variable to hold path of scanned file
  treenode_data *pt_data = filepath_to_treenode_data(input_args->path);
  scanned_path *spbuf;
  //add it to the tree
  add_rbtree(tree, &pt_data);
  spbuf = pt_data->data.file;
  //if this path haven't been scanned yet
  if(spbuf->head == NULL){
    //set its info list head and tail
    spbuf->head = fsbuf;
    spbuf->tail = fsbuf;
  }else{
    //otherwise add at the end current file stats
    spbuf->tail->next = fsbuf;
    spbuf->tail = fsbuf;
  }
  //if user, group or length options are passed, handle it
  if(HAS_FILTER(fsconf.hasopt, FILTER)){
    //if any of the possibile filters change this variable,
    int is_printable = 0;
    if(HASOPT(fsconf.hasopt, USER)){
      if(fsbuf->uid == fsconf.user)
        is_printable = 1;
    }
    if(HASOPT(fsconf.hasopt, GROUP)){
      if(fsbuf->gid == fsconf.group)
        is_printable = 1;
    }
    if(HASOPT(fsconf.hasopt, LENGTH)){
      if((fsbuf->size > fsconf.length_min)
      && (fsbuf->size < fsconf.length_max)){
        is_printable = 1;
      }
    }
    //print the current file stats
    if(is_printable)
      printFstat(*fsbuf, input_args->path);
  }
  //if verbose option is passed, print file stats
  if(HASOPT(fsconf.hasopt, VERBOSE)){
	   printFstat(*fsbuf, input_args->path);
  }

  if(HASOPT(fsconf.hasopt, STAT)){
    //update program stats
    update_stats_size(fsbuf->size);
    update_stats_type(fsbuf->mode);
  }

  if(HASOPT(fsconf.hasopt, REPORT)){
    //update program report
    update_report(fsbuf->size);
  }
  //if this argument has 'r' option
  if(HASOPT(input_args->options, RECURSIVE)){
    //and if this file is a directory
    if(S_ISDIR(fsbuf->mode)){
      //then if this argument has 'l' option too
      //don't scan this directory subdirectories
      //if it has been already scanned
      if(HASOPT(input_args->options, FOLLOW_LINK)){
        treenode_data *inode_data = inode_to_treenode_data(stbuf.st_ino);
        //if tree doesn't contain this path
        if(get_data_rbtree(inode_tree, inode_data) == NULL){
          add_rbtree(inode_tree, &inode_data);
          dirwalk(input_args, filestat);
        }else{
          free(inode_data);
        }
      }else{//if this argument does not have 'l' option
        dirwalk(input_args, filestat);
      }
    }
  }
}
/*
  this function takes a input argument and a pointer to a function,
  then applies that function to every subdirectory of the directory
  indicated by the argument
*/
void dirwalk(input_file_argument *dir, void (*fcn)(input_file_argument *))
{
  char *dirpath;
  char *filepath;
  struct dirent *dp;
  DIR *dfd;

  dirpath = dir->path;

  if((dfd = opendir(dir->path)) == NULL){
    fprintf(stderr, "dirwalk: can't open %s\n", dir->path);
    return;
  }

  while((dp = readdir(dfd)) != NULL){
    // printf("dirwalk: reading %s\n", dp->d_name);
    //ignore itself and its parent
    if(strcmp(dp->d_name, ".") == 0
    || strcmp(dp->d_name, "..") == 0)
      continue;
    if((filepath = malloc(sizeof(char) * (strlen(dirpath) + strlen(dp->d_name) + 2))) == NULL){
      fprintf(stderr, "%s", MALLOC_ERROR);
      exit(1);
    }
    sprintf(filepath, "%s/%s", dirpath, dp->d_name);
    // free(dir->path);
    dir->path=filepath;
    (*fcn)(dir);
  }
  closedir(dfd);
}

file_info statcpy(struct stat *stbuf)
{
  file_info fsbuf;

  time_t rawtime;
  time(&rawtime);

  fsbuf.date = rawtime;
  fsbuf.size = stbuf->st_size;
  fsbuf.mode = stbuf->st_mode;
  fsbuf.atime = stbuf->st_atime;
  fsbuf.ctime = stbuf->st_ctime;
  fsbuf.mtime = stbuf->st_mtime;
  fsbuf.nlink = stbuf->st_nlink;
  fsbuf.uid = stbuf->st_uid;
  fsbuf.gid = stbuf->st_gid;
  fsbuf.next = NULL;

  return fsbuf;
}
