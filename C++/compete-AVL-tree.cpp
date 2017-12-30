// 构建一个AVL树：
// AVL树：本身是一棵二叉搜索树，每个节点的左右子树高度之差，最多为1 
#include<iostream>
using namespace std ; 
struct node{ 
    int val ; 
    struct node *left, *right ;
    node(int v) : val(v), left(NULL), right(NULL) {} 
} ;

int keys[10] = {0,1,2,3,4,5,6,7,8,9} ; 
int num = 10 ; 

int treeheight(struct node *root) {
    if ( !root ) 
        return 0 ;
    return 1 + max(treeheight(root->left),treeheight(root->right)) ; 
}

struct node* L(struct node *root) {
    struct node *temp = root->right ; 
    root->right = temp->left ; 
    temp->left = root ; 
    return temp ; 
}

struct node* R(struct node *root) {
    struct node *temp = root->left ; 
    root->left = temp->right ; 
    temp->right = root ;
    return temp ; 
}

struct node* LR(struct node *root ) {
    root->left = L(root->left) ; 
    root = R(root) ; 
    return root ; 
}

struct node* RL(struct node *root ) {
    root->right = R(root->right) ; 
    root = L(root) ; 
    return root ; 
}

struct node *buildAVLtree(struct node *root, int value ) {
    if ( !root ) {
        root = new struct node(value) ; 
        return root ; 
    }
    if ( value < root->val ) {                           // 插入的节点在根节点的左子树 
        root->left = buildAVLtree(root->left,value) ; 
        int l = treeheight(root->left) ; 
        int r = treeheight(root->right) ; 
        if ( l - r >= 2 ) {
            if ( value < root->left->val ) 
                root = R(root) ;                         // 直接右旋转
            else 
                root = LR(root) ;                        // 它的左子树先左旋转，它自己再右旋转
        }
    }
    else {
        root->right = buildAVLtree(root->right,value) ;  // 插入的节点在根节点的右子树 
        int l = treeheight(root->left) ;  
        int r = treeheight(root->right) ; 
        if ( r - l >= 2 ) {
            if ( value > root->right->val )               // 直接左旋转
                root = L(root) ; 
            else 
                root = RL(root) ;                          // 它的右子树先右旋转，它自己再左旋转 
        }
    }
    return root ; 
}

int main() {
    struct node *root = NULL ;  
    for ( int i = 0 ; i < num ; i++ ) 
        root = buildAVLtree(root,keys[i]) ; 
    return 0 ; 
}