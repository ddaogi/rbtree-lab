#include "rbtree.h"
#include <stdio.h>
#include <stdlib.h>

void left_rotate(rbtree *t, node_t *x){
  node_t *y = x->right;
  x->right = y->left;

  // y의 왼쪽자식이 존재 한다면
  if( y-> left != t->nil){
    y->left->parent = x;
  }

  // left rotate 시 부모 자식 관계에 변화를 반영한다.
  // y의 부모를 x에서 x의 부모로 변경해준다.
  y->parent = x->parent;

  if( x-> parent == t->nil){
    t -> root =y;
  }
  // x의 부모가 nil이 아니고, x가 그 부모의 왼쪽 자식이었다면 
  else if( x== x->parent->left){
    x->parent->left = y;
  }
  // x 가 그 부모의 오른쪽 자식이었다면
  else{
    x->parent->right = y;
  }
  y->left = x;
  x->parent = y;
}
void right_rotate(rbtree *t, node_t *y){
  node_t *x = y->left;
  y->left = x->right;

  //x의 오른쪽 자식이 존재한다면
  if(x->right != t->nil){
    x->right->parent = y; // 오른쪽자식의 부모를 y로 변경해준다.
  }
  //x의 부모를 y의 부모로 변경한다.
  x->parent = y->parent;

  //이 때 y->parent == t->nil 이면,
  if (y->parent == t->nil){
    //새로운 root는 x가 된다.
    t->root = x;
  }

  //y가 root가 아니고, y가 왼쪽 자식이었다면
  else if ( y == y->parent->left){
    // y의 부모의 새로운 왼쪽자식을 x로 연결한다.
    y->parent->left = x;
  }
  //y가 오른쪽 자식이었다면
  else{
    //y의 부모의 새로운 오른쪽 자식을 x로 연결한다.
    y->parent->right = x;
  }
  x->right = y;
  y->parent = x;

}
// 새로운 rbtree를 생성하고
// root, nil 정보를 가진 포인터를 반환하는 함수
rbtree *new_rbtree(void)
{
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  node_t *nil_node = (node_t *)calloc(1, sizeof(node_t));

  p->nil = nil_node;    // 포인터 변수와 nil노드를 이어준다.
  p->root = p->nil;             // root, nil노드를 이어준다. (아직 삽입된 노드가 없을 때의 초기 ver.)
  p->nil->color = RBTREE_BLACK; // nil노드는 항상 Black이므로, 색상을 지정해준다.

  return p; //새로운 rbtree 정보를 가진 포인터 변수 p를 반환한다.
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  //rbtree erase를 먼저 하고, 그다음에 delete를 해야함
  free(t);
}

void rbtree_insert_fixup(rbtree *t, node_t *z){

  node_t *uncle = NULL;

  while( z->parent->color == RBTREE_RED){

    //부모가 할아버지의 왼쪽 자식 일 때,
    if (z->parent == z->parent->parent->left){
      // 삼촌은 오른쪽 자식이 된다
      uncle = z->parent->parent->right;

  
    if (uncle->color == RBTREE_RED){
      // parent와 uncle 모두 black으로 바꾼다.
      z->parent->color = RBTREE_BLACK;
      uncle->color = RBTREE_BLACK;

      //할아버지를 red로 바꾼 뒤, 포인터를 grandparent로 옮김
      z->parent->parent->color = RBTREE_RED;
      z= z->parent->parent;
    }
    else
    {
      // 2) 부모 레드 & 삼촌 블랙 & 내가 RIGHT 자식 일 때
      if ( z== z->parent->right){
        // 포인터 parent로 올리고, left-rotate
        z = z->parent;
        left_rotate(t, z);
      }
      // 3) 부모 레드 & 삼촌 블랙 & 내가 LEFT 자식, PARENT는 BLACK 으로 할아버지를 RED로
      z->parent->color = RBTREE_BLACK;
      z->parent->parent->color = RBTREE_RED;

      // 포인터를 할아버지로 옮겨서 right-rotate
      right_rotate(t, z->parent->parent);
      }
    }
    //부모가 할아버지의 오른쪽 자식일 때
    else{
      uncle = z -> parent -> parent -> left;
      if(uncle -> color == RBTREE_RED){
        z->parent->color = RBTREE_BLACK;
        uncle->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z= z->parent->parent;
      }
      else{
        if ( z == z->parent->left){
          z = z->parent;
          right_rotate(t,z);
        }
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        left_rotate(t, z->parent->parent);
      }
    }
  }
  //insert 할 노드가 root 노드일 때 색만 바꿔주면 해결
  t->root->color = RBTREE_BLACK;
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // 삽입할 key값을 가진 노드를 만들어준다
  node_t *z = (node_t *)calloc(1,sizeof(node_t));
  z->key = key;

  //자리를 찾아 내려갈 ptr 변수 x,y
  node_t *x = t->root;
  node_t *y = t->nil;

  while( x!= t->nil){
    y = x;
    if (x->key >key){
      x = x->left;
    }
    else{
      x = x->right;
    }
  }
  z->parent = y;
  
  if( y== t->nil){
    t->root = z;
  }
  // nil노드가 아니라면, key 값의 크기에 따라 left, right 자리를 정해준다.

  else if( y->key < key){
    y->right = z;
  }
  else{
    y->left = z;
  }

  z-> color = RBTREE_RED;
  z-> left = t->nil;
  z-> right = t->nil;


  rbtree_insert_fixup(t,z);

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
