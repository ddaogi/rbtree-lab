#include <stdio.h>
#include <stdlib.h>
#include "rbtree.h"

int main(int argc, char *argv[]) {
    rbtree* tree1 = new_rbtree();
    rbtree_insert(tree1,50);
    rbtree_insert(tree1,30);
    rbtree_insert(tree1,20);
    
    node_t* min_node = rbtree_min(tree1);
    printf("%d\n", min_node->key);
    node_t* max_node = rbtree_max(tree1);
    printf("%d\n", max_node->key);
    
    node_t* found = rbtree_find(tree1,23);
    printf("%d\n", found->key);
    return 0;
}