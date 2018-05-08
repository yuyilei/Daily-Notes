#include<iostream>
using namespace std ; 

int father[100] ; 

int root(int a) {
    if ( a != father[a] ) 
        father[a] = root(father[a]) ;          // 查找根节点，并压缩路径
    return father[a] ;   
}

void Union(int a, int b) {                   
    int x = root(a) ;                          // 查看a的根节点
    int y = root(b) ;                          // 查看b的根节点  
    if ( father[x] != father[y] )              // 如果两者的根节点不同，就把x的根节点设为y，意为合并两点 
        father[root(x)] = root(y) ;  
}

int main() {
    int n, i, a, b ; 
    for ( i = 0 ; i < n ; i++ ) {
        cin >> a >> b ;                        // a,b两点之间有连接  
        Union(a,b) ;                           // 合并a,b两点  
    }
}