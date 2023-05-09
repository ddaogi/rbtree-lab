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


rbtree *new_rbtree(void)
{
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  p->nil =(node_t *)calloc(1, sizeof(node_t));  // 포인터 변수와 nil노드를 이어준다.
  p->root = p->nil;             // root, nil노드를 이어준다. (아직 삽입된 노드가 없을 때의 초기 ver.)
  p->nil->color = RBTREE_BLACK; // nil노드는 항상 Black이므로, 색상을 지정해준다.
  return p; //새로운 rbtree 정보를 가진 포인터 변수 p를 반환한다.
}

void postorder_del(node_t * root, node_t *nil){
  if( root == NULL|| root == nil) return;
  postorder_del(root->left, nil);
  postorder_del(root->right, nil);
  if (root != nil) free(root);
}

void delete_rbtree(rbtree *t){
  postorder_del(t->root, t->nil);
  free(t->nil);
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
  // if(rbtree_find(t,key) != NULL){
  //   return t->root;
  // }
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
  for(ptr = t->root; ptr->left!=t->nil; ptr=ptr->left){
  }
  return ptr;
}

node_t *rbtree_max(const rbtree *t) {
  node_t* ptr;
  for(ptr = t->root; ptr->right!=t->nil; ptr=ptr->right){
  }
  return ptr;
}

// 부모 자식 관계를 재설정 해주는 함수
void rb_transplant(rbtree *t, node_t *u , node_t *v){

  // 지우려는 노드가 root 면, 자식을 root로
  if (u ->parent == t->nil){
    t->root = v;
  }
  // 지우려는 노드가 왼쪽 자식이면, 부모의 왼쪽 자식 노드를 왼쪽 자식으로 연결
  else if (u == u->parent->left){
    u->parent->left = v;
  }
  // 지우려는 노드가 오른쪽 자식이면, 부모의 오른쪽자식노드를 오른쪽 자식으로 연결
  else{
    u->parent->right = v;
  }
  v->parent = u->parent;
  
}
node_t *subtree_min(const rbtree *t, node_t *z)
{
  node_t *ptr = z;
  while (ptr->left != t->nil)
  {
    ptr = ptr->left;
  }
  return ptr;
}

void rb_erase_fixup(rbtree *t, node_t* x){
  //x 는
  node_t *bro = t->nil;

  // root가 아니고 doubly black 일때
  while( x != t->root && x->color == RBTREE_BLACK){
    
    // x가 왼쪽 자식이면
    if ( x== x->parent->left){
      bro = x->parent->right;
      // 형제가 레드 이면 case1
      if (bro->color == RBTREE_RED){
        bro->color = RBTREE_BLACK; // 형제 black으로 바꾸고
        x->parent->color = RBTREE_RED; // 부모 red로 바꾸고
        left_rotate(t, x->parent);
        bro = x->parent->right;
      }
      // 형제와 자식 다 블랙일경우 case2
      if(bro->left->color == RBTREE_BLACK && bro->right->color == RBTREE_BLACK){
        bro->color = RBTREE_RED;
        x = x->parent;
      }
      // 형제가 블랙일 때
      else{
        if(bro->right->color == RBTREE_BLACK){
          bro->left->color = RBTREE_RED;
          right_rotate(t,bro);
          bro = x->parent->right;
        }
        // 형제 블랙, 오른쪽 자식 레드 case4
        bro->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        bro->right->color = RBTREE_BLACK;
        left_rotate(t, x->parent);
        x = t->root;
      }
    }
    
    //x가 오른쪽 자식이면
    else{
      bro = x->parent->left;
      if( bro->color == RBTREE_RED){
        bro->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        right_rotate(t, x->parent);
        bro = x->parent->left;
      }
      if( bro->right->color == RBTREE_BLACK && bro->left->color == RBTREE_BLACK){
        bro->color = RBTREE_RED;
        x = x->parent;
      }
      else{
        if(bro->left->color == RBTREE_BLACK){
          bro->right->color = RBTREE_BLACK;
          bro->color= RBTREE_RED;
          left_rotate(t,bro);
          bro = x->parent->left;
        }
         
        bro->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        bro->left->color = RBTREE_BLACK;
        right_rotate(t, x->parent);
        x = t->root;
        }
      }
    }
    // 모든 조건을 만족, x가루트였고 red인경우 블랙으로 변경
  x-> color = RBTREE_BLACK;
  
}
int rbtree_erase(rbtree *t, node_t *z) {
  //x는 successor의 원래 위치로 이동하는 노드
  //node y 는 트리에서 삭제된 노드, or 트리에서 이동한 노드를 가리킨다
  node_t *x = NULL;
  node_t *y = z;
  color_t y_origin = y->color;

  // 1) 지우려는 노드의 오른쪽 자식이 있을 수 있을 때
  if (z->left == t->nil){
    x= z->right;
    rb_transplant(t,z,z->right);
  }

  // 2)  지우려는 노드의 왼쪽 자식이 있을 수 있을 때
  else if( z->right == t->nil){
    x = z->left;
    rb_transplant(t,z,z->left);
  }

  // 3) 지우려는 노드의 왼쪽, 오른쪽 자식 모두 있을때
  else{
    // y는 지우려는 노드의 success를 가리키도록 한다.
    y = subtree_min(t,z->right);
    y_origin = y->color;
    x = y->right;

    //3-1 successor y가 직속 자식에 바로 있을 때
    if (y->parent == z){
      x->parent = y;
    }

    else{
      // 'y의 오른쪽 자식 <-> y의 부모간 부자 관계 설정을 다시 한다.
      // successor를 옮기기 전 자식노드들의 새로운 부모를 연결해야 하기 때문이다.
      rb_transplant(t,y,y->right);
      y->right = z->right;
      y->right->parent = y;
    }
    // 'y <-> z의 부모간 부자관계 설정을 다시 한다.
    rb_transplant(t,z,y);

    y->left = z->left;
    y->left->parent = y;

    y->color = z->color;
  }

  //삭제한 color가 doubly black 이면
  // 2,4,5 번 조건을 위반할 수 있으므로 고치러 보낸다
  if (y_origin == RBTREE_BLACK){
    rb_erase_fixup(t,x);
  }
  free(z);
  return 0;
}

int rb_inorder(node_t *root, key_t *res, const rbtree *t, int i){
  if(root == t->nil){
    return i;
  }
  i = rb_inorder(root->left,res,t,i);
  res[i] = root->key;
  i = rb_inorder(root->right,res,t,i+1);
  return i;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  rb_inorder(t->root,arr,t,0);
  return 0;
}

