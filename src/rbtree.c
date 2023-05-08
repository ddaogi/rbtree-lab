#include "rbtree.h"
#include <stdio.h>
#include <stdlib.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  p->root = NULL;
  return p;
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  //rbtree erase를 먼저 하고, 그다음에 delete를 해야함
  free(t);
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  if(t->root == NULL){
    t->root = (node_t*) calloc(1,sizeof(node_t));
    t->root->color = 1; // black
    t->root->key = key;
    t->root->parent = NULL;
    t->root->right = NULL;
    t->root->left = NULL;
  }
  else{
    node_t* ptr = t->root;
    while(1){
      
      if(ptr->key< key){
        if(ptr->right == NULL){
          node_t* new_node = (node_t*) calloc(1,sizeof(node_t));
          ptr->right = new_node;
          new_node->parent = ptr;
          new_node->key = key;
          new_node->right = NULL;
          new_node->left = NULL;
          new_node->color = 0; // red
          break;
        }
        else{
          ptr = ptr->right;
        }
      }
      else{
        if(ptr->left == NULL){
          node_t* new_node = (node_t*) calloc(1,sizeof(node_t));
          ptr->left = new_node;
          new_node->parent = ptr;
          new_node->key = key;
          new_node->right = NULL;
          new_node->left = NULL;
          new_node->color = 0; // red
          break;
        }
        else{  
          ptr = ptr->left;
        }
      }

    }
  }
  return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  node_t* ptr= t->root;
  while(1){
    if(ptr->key > key){
      if(ptr->left == NULL){
        return NULL;
      }
      ptr = ptr->left;
    }
    else if(ptr->key < key){
      if(ptr->right == NULL){
        return NULL;
      }
      ptr = ptr->right;
    }
    else{
      return ptr;
    }
  }
  
}

node_t *rbtree_min(const rbtree *t) {
  node_t* ptr;
  for(ptr = t->root; ptr->left!=NULL; ptr=ptr->left){
  }
  return ptr;
}

node_t *rbtree_max(const rbtree *t) {
  node_t* ptr;
  for(ptr = t->root; ptr->right!=NULL; ptr=ptr->right){
  }
  return ptr;
}

int rbtree_erase(rbtree *t, node_t *p) {
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {

  return 0;
}
