#include<iostream>
#include<cstdio>
#include<queue>
using namespace std ;
struct node {
    int val ;
    short color ; 
    struct node *parent, *left, *right ; 
    node(int v) : val(v), color(1), parent(NULL), left(NULL), right(NULL) {}
} ; 

int keys[10] = {2,1,5,10,15,120,8,34,23,4} ; 
int num = 10 ; 
struct node *root = NULL ;                      // 全局变量，作为红黑树的根 

struct node *father(struct node *one) ;         // 返回父节点 
struct node *grandfather(struct node *one) ;    // 返回祖父节点
struct node *uncle(struct node *one) ;          // 返回叔叔节点
void insertRB(struct node *one) ;               // 插入一个节点到红黑树
void insert_recurse(struct node *one) ;         // 将一个节点插入到二叉树
void insert_repair(struct node *one) ;          // 讲二叉树修复为红黑树
void insert_fix_case1(struct node *one) ;       // 修复情况1
void insert_fix_case2(struct node *one) ;       // 修复情况2  
void insert_fix_case3(struct node *one) ;       // 修复情况3   
void insert_fix_case4(struct node *one) ;       // 修复情况4 
void insert_fix_case4step2(struct node *one) ;  // 修复情况4中的第二步   
void L(struct node *one) ;                      // 左旋转 
void R(struct node *one) ;                      // 右旋转
void levelorder();                              // 层次遍历  

int main() {
    int i ;
    for ( i = 0 ; i < num ; i++ ) {
        struct node *tmp = new struct node(keys[i]) ; 
        insertRB(tmp) ; 
        cout << "\n" << endl ; 
        levelorder() ; 
    }
    return 0 ; 
}

void insertRB(struct node *one) {
    insert_recurse(one) ;
    insert_repair(one) ; 
}

struct node *father(struct node *one) {
    return one->parent ; 
}

struct node *grandfather(struct node *one) {
    if ( !father(one) ) 
        return NULL ; 
    return father(father(one)) ; 
}

struct node *uncle(struct node *one ) {
    struct node *f = father(one) ; 
    struct node *gf = grandfather(one) ; 
    if ( !gf ) 
        return NULL ; 
    if ( f == gf->left ) 
        return gf->right ; 
    return gf->left ; 
}

void insert_recurse(struct node *one) {
    struct node *r = root ; 
    struct node *f = NULL ; 
    while ( r ) {
        f = r ; 
        if ( one->val < r->val ) 
            r = r->left ; 
        else 
            r = r->right ;    
    }   
    one->parent = f ; 
    if ( f ) {
        if ( one->val < f->val ) 
            f->left = one ;
        else 
            f->right = one ; 
    }
    insert_repair(one) ;  
}

void insert_repair(struct node *one) {
    if ( !father(one) ) 
        insert_fix_case1(one) ;
    else if ( father(one)->color == 0 ) 
        insert_fix_case2(one) ; 
    else if ( uncle(one) && uncle(one)->color == 1 ) 
        insert_fix_case3(one) ; 
    else if ( !uncle(one) || uncle(one)->color == 0 ) 
        insert_fix_case4(one) ; 
} 

void insert_fix_case1(struct node *one) {              // case1: 插入的节点是根节点 
    one->color = 0 ; 
    root = one ; 
}

void insert_fix_case2(struct node *one) {              // case2: 插入的节点的父节点是黑色的
    return ; 
}

void insert_fix_case3(struct node *one) {              // case3: 父节点是红色，叔叔节点是红色 
    father(one)->color = 0 ;
    uncle(one)->color = 0 ;
    grandfather(one)->color = 1 ; 
    insert_repair(grandfather(one)) ; 
}

void insert_fix_case4(struct node *one) {             // case4: 父节点是红色，叔叔节点是是黑色 
    struct node *f = father(one) ; 
    struct node *gf = grandfather(one) ;
    if ( gf->left && one == gf->left->right ) {
        L(f) ; 
        one = one->left ; 
    } 
    else if ( gf->right && one == gf->right->left ) {
        R(f) ; 
        one = one->right ; 
    }
    insert_fix_case4step2(one) ; 
}

void insert_fix_case4step2(struct node *one) {
    struct node *f = father(one) ;
    struct node *gf = grandfather(one) ; 
    if ( one == f->left ) 
        R(gf) ; 
    else 
        L(gf) ; 
    f->color = 0 ; 
    gf->color = 1 ; 
}

void L(struct node *one) {
    struct node *temp = one->right ; 
    one->right = temp->left ; 
    if ( temp->left ) 
        temp->left->parent = one ;
    temp->left = one ; 
    if ( !father(one) )
        root = temp ;
    else {
        if ( one == father(one)->left )
            father(one)->left = temp ; 
        else 
            father(one)->right = temp ; 
    } 
    temp->parent = one->parent ; 
    one->parent = temp ; 
}

void R(struct node *one) {
    struct node *temp = one->left ;
    one->left = temp->right ; 
    if ( temp->right ) 
        temp->right->parent = one ; 
    temp->right = one ; 
    if ( !father(one) ) {
        root = temp ; 
    } 
    else {
        if ( one == father(one)->right ) 
            father(one)->right = temp ;
        else 
            father(one)->left = temp ; 
    } 
    temp->parent = one->parent ; 
    one->parent = temp ; 
}

void levelorder() {
    queue<struct node*> q ;  
    struct node *tree = root ; 
    if ( !tree )    
        return ; 
    q.push(tree) ; 
    while ( !q.empty() ) {
        int len = q.size() ;
        for ( int i = 0 ; i < len ; i++ ) {
            struct node *tmp = q.front() ; 
            q.pop() ; 
            if ( tmp->left ) q.push(tmp->left) ; 
            if ( tmp->right ) q.push(tmp->right) ;
            cout << "(" << tmp->val << "," << tmp->color << "," << ((tmp->parent)?(tmp->parent->val):0) << ") " ;   
        }
        cout << endl ; 
    } 
}
