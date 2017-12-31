#include<iostream>
#include<queue> 
using namespace std ; 

int keys[10] = {2,1,5,10,15,120,8,34,23,4} ; 
int num = 10 ; 
struct node {
    int val, color ; 
    struct node *father, *left, *right ; 
    node(int v) : val(v), color(1), father(NULL), left(NULL), right(NULL) {}  
} ; 
struct node *tree = NULL ;              // 全局变量，作为红黑树的根 
void L(struct node *root) ;             // 左旋转
void R(struct node *root) ;             // 右旋转
void fix_to_RB(struct node *one) ;      // 将二叉查找树修复为红黑树
void buildtree(struct node *one ) ;     // 将一个节点插入二叉树 

// node中color为1表示红色，为0表示黑色。 
// case1 : 父节点是祖父节点的左孩子,父节点是红色，叔叔节点存在，且是红色。 
// case2 : 父节点是祖父节点的左孩子,父节点是红色，叔叔节点不存在或者为黑色，当前节点是父节点的右孩子。
// case3 : 父节点是祖父节点的左孩子,父节点是红色，叔叔节点不存在或者为黑色，当前节点是父节点的左孩子。
// case4 : 父节点是祖父节点的右孩子,父节点是红色，叔叔节点存在，且是红色。  
// case5 : 父节点是祖父节点的右孩子,父节点是红色，叔叔节点不存在或者为黑色，当前节点是父节点的右孩子。
// case6 : 父节点是祖父节点的右孩子,父节点是红色，叔叔节点不存在或者为黑色，当前节点是父节点的左孩子。

int main() {
    for ( int i = 0 ; i < num ; i++ ) {
        struct node *insert = new struct node (keys[i]) ; 
        if ( !insert ) 
            return 0 ; 
        buildtree(insert) ; 
    }
    queue<struct node*> q;                                 // 层次遍历红黑树 
    q.push(tree) ; 
    while ( !q.empty() ) {
        int len = q.size() ;
        for ( int i = 0 ; i < len ; i++ ) {
            struct node *tmp = q.front() ; 
            q.pop() ; 
            if ( tmp->left ) q.push(tmp->left) ; 
            if ( tmp->right ) q.push(tmp->right) ;
            cout << "(" << tmp->val << "," << tmp->color << "," << ((tmp->father)?(tmp->father->val):0) << ") " ;   
        }
        cout << endl ; 
    } 
    return 0 ; 
}

void L(struct node *root) {                        // 左旋转
    struct node *temp = root->right ;
    root->right = temp->left ; 
    if ( temp->left )
        temp->left->father = root ; 
    temp->father = root->father ;
    if ( root->father == NULL ) 
        tree = temp ; 
    else {
        if ( root == root->father->left ) 
            root->father->left = temp ; 
        else 
            root->father->right = temp ; 
    }  
    temp->left = root ; 
    root->father = temp ; 
}

void R(struct node *root ) {                      // 右旋转 
    struct node *temp = root->left ; 
    root->left = temp->right ; 
    if ( temp->right )
        temp->right->father = root ; 
    temp->father = root->father ; 
    if ( root->father == NULL ) 
        tree = temp ; 
    else {
        if ( root == root->father->right ) 
            root->father->right = temp ; 
        else 
            root->father->left = temp ; 
    } 
    temp->right = root ; 
    root->father = temp ; 
}

void fix_to_RB(struct node* one) {                  
    struct node *f, *gf, *uncle ; // f为当前节点的父节点，gf为当前节点的祖父，uncle是当前节点的叔叔节点  
    while ( (f = one->father) && (f->color == 1) ) { 
        uncle = NULL ;
        gf = f->father ;
        if ( f == gf->left ) {
            uncle = gf->right ; 
            if ( (uncle != NULL ) && (uncle->color == 1) ) {     // case1 
                f->color = 0 ;                                   //  父节点设置为黑色 
                uncle->color = 0 ;                               //  叔叔节点设置为黑色
                gf->color = 1 ;                                  //  祖父节点设置为红色
                one = gf ;                                       //  将祖父节点设为当前节点 
            }
            else {
                if ( one == f->right ) {                         // case2 
                    L(f) ;                                       // 对父节点进行左旋转 -- 就是将前节点转到父节点的位置，父节点变为当前节点的左孩子。
                    struct node *tmp = f ;                       // 交换当前节点和父节点的指针
                    f = one ;                                    // 当前节点变为父节点
                    one = tmp ;  
                } 
                                                                 //  case2和case3 
                f->color = 0 ;                                   //  父节点设为黑色
                gf->color = 1 ;                                  //  祖父节点设为红色
                R(gf) ;                                          //  右旋转祖父节点 -- 就是将父节点转到祖父节点的位置，组父节点变为父节点的右孩子。 -- 现在的祖父节点变为下一次循环中当前节点的叔叔，且叔叔是红色的。
            }
        }
        else {
            uncle = gf->left ; 
            if ( (uncle != NULL ) && (uncle->color == 1) ) {     // case4
                f->color = 0 ;                                   // 父节点设置为黑色 
                uncle->color = 0 ;                               // 叔叔节点设置为黑色
                gf->color = 1 ;                                  // 祖父节点设置为红色
                one = gf ;                                       // 将祖父节点设为当前节点
            } 
            else {
                if ( one == f->left ) {                         // case6
                    R(f) ;                                      // 右旋转父节点 -- 就是将当前节点转到父节点的位置，父节点变为当前节点的右孩子
                    struct node *tmp = f ;                      // 交换当前节点和父节点的指针
                    f = one ;                                   // 当前节点变为父节点 
                    one = tmp ;  
                }                                                   
                                                                // case5和case6
                f->color = 0 ;                                  // 父节点设为黑色
                gf->color = 1 ;                                 // 祖父节点设为红色
                L(gf) ;                                         // 左旋转祖父节点 -- 就是将父节点转到祖父节点的位置，组父节点变为父节点的左孩子。 -- 现在的祖父节点变为下一次循环中当前节点的叔叔，且叔叔是红色的。 
            }
        } 
    }
    tree->color = 0 ; 
}

void buildtree(struct node *one ) { 
    struct node *f = NULL ; 
    struct node *r = tree ; 
    while ( r != NULL ) {
        f = r ; 
        if ( one->val < r->val ) 
            r = r->left ; 
        else 
            r = r->right ; 
    } 
    one->father = f ; 
    if ( f == NULL ) {              // 父节点为空，说明插入的是根节点 
        one->color = 0 ; 
        tree = one ;  
        return ; 
    } 
    if ( f->val > one->val )        // 判断插入的节点是父节点的左孩子还是右孩子
        f->left = one ;
    else 
        f->right = one ; 
    fix_to_RB(one) ;                // 修复为红黑树 
}

