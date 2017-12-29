// 从一棵树的先序遍历和中序遍历获得后序遍历的顺序 
#include<iostream>
#include<vector>
using namespace std ; 
int pre[7] = {1,2,3,4,5,6,7} ; 
int in[7] = {2,3,1,5,4,7,6} ;
vector<int> post ;  
int num = 7 ; 

void get_postorder(int ps, int pe, int is, int ie) {
    if ( ps == pe ) {
        post.push_back(pre[ps]) ; 
        return ; 
    }
    int index = is ;
    for ( ; index < ie && in[index] != pre[ps] ; index++ ) ; 
    int len = index - is ; 
    if ( index != is )                             // 存在左子树时，构建左子树  
        get_postorder(ps+1,ps+len,is,index-1) ; 
    if ( index != ie )                             // 存在右子树时，构建右子树
        get_postorder(ps+len+1,pe,index+1,ie) ; 
    post.push_back(pre[ps]) ;                      // 最后是根节点 
}

int main() {
    get_postorder(0,num-1,0,num-1) ; 
    for ( int i = 0 ; i < num ; i++ ) 
        cout << post[i] << " " ; 
    cout << endl ; 
    return 0 ; 
}