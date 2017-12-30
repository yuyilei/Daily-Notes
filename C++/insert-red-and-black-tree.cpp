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
struct node *tree = NULL ; 

void L(struct node *root) {
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

void R(struct node *root ) {
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
    struct node *f, *gf, *uncle ; 
    while ( (f = one->father) && (f->color == 1) ) {
        uncle = NULL ;
        gf = f->father ;
        if ( f == gf->left ) {
            uncle = gf->right ; 
            if ( (uncle != NULL ) && (uncle->color == 1) ) {
                f->color = 0 ; 
                uncle->color = 0 ; 
                gf->color = 1 ; 
                one = gf ; 
            }
            else {
                if ( one == f->right ) {
                    L(f) ;
                    struct node *tmp = f ; 
                    f = one ; 
                    one = tmp ;  
                } 
                f->color = 0 ; 
                gf->color = 1 ; 
                R(gf) ; 
            }
        }
        else {
            uncle = gf->left ; 
            if ( (uncle != NULL ) && (uncle->color == 1) ) {
                f->color = 0 ; 
                uncle->color = 0 ; 
                gf->color = 1 ; 
                one = gf ; 
            } 
            else {
                if ( one == f->left ) {
                    R(f) ;
                    struct node *tmp = f ; 
                    f = one ; 
                    one = tmp ;  
                } 
                f->color = 0 ; 
                gf->color = 1 ;
                L(gf) ; 
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
    if ( f == NULL ) {
        one->color = 0 ; 
        tree = one ;  
        return ; 
    } 
    if ( f->val > one->val ) 
        f->left = one ;
    else 
        f->right = one ; 
    fix_to_RB(one) ; 
}

int main() {
    for ( int i = 0 ; i < num ; i++ ) {
        struct node *insert = new struct node (keys[i]) ; 
        if ( !insert ) 
            return 0 ; 
        buildtree(insert) ; 
    }
    queue<struct node*> q; 
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