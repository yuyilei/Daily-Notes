// 从一棵树后序遍历和中序遍历得到它的先序遍历的顺序 
#include<iostream>
#include<vector>
using namespace std ; 

int post[7] = {3,2,5,7,6,4,1} ; 
int in[7] = {2,3,1,5,4,7,6} ;
int num = 7 ; 
vector<int> pre ; 

void get_preorder(int ps, int pe, int is, int ie) {
    if ( ps == pe ) {
        pre.push_back(post[pe]) ; 
        return ; 
    }
    int index = is ; 
    for ( ; index <= ie && in[index] != post[pe] ; index++ ) ; 
    pre.push_back(post[pe]) ;
    int len = ie - index ;                                      // 先放入根节点  
    if ( index != is ) get_preorder(ps,pe-len-1,is,index-1) ;   // 左子树存在，就放入左子树
    if ( index != ie ) get_preorder(pe-len,pe-1,index+1,ie) ;   // 右子树存在，就放入右子树  
}

int main() {
    get_preorder(0,num-1,0,num-1) ; 
    for ( int i = 0 ; i < num ; i++ ) 
        cout << pre[i] << " " ; 
    cout << endl ; 
    return 0 ; 
} 
