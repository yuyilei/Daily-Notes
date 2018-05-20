#include<cstdio>
#include<iostream>
using namespace std ; 

/*
构造哈密顿回路(回溯) 
哈密顿回路：从一个节点出发，其余节点恰好只经过一次，最后回到上一个节点
*/

class Hamilton {
    private : 
        int n ;                                        // 图中的节点个数
        int **arc ;                                    // 邻接矩阵
        int *x ;                                       // 存结果的数组 
        bool *visit ;                                 
    public : 
        Hamilton() {
            printf("输入节点个数:\n") ; 
            scanf("%d",&n) ; 
            arc = new int *[n] ; 
            x = new int [n] ; 
            visit = new bool [n] ; 
            for ( int i = 0 ; i < n ; i++ ) {
                arc[i] = new int [n] ; 
            }
            printf("输入邻接矩阵:\n") ; 
            for ( int i = 0 ; i < n ; i++ ) {
                for ( int j = 0 ; j < n ; j++ ) {
                    scanf("%d",&arc[i][j]) ; 
                }
            }
        }

        void showCircle() {
            printf("(") ; 
            for ( int i = 0 ; i < n ; i++ ) {
                printf(" %d",x[i]+1) ; 
            }
            printf(" )\n") ; 
        }

        void HamiltonCircle(){
            int i, j ; 
            for ( i = 0 ; i < n ; i++ ) {
                x[i] = 0 ; 
                visit[i] = false ; 
            }
            x[0] = 0 ; 
            visit[0] = true ; 
            int k = 1 ; 
            while ( k >= 1 ) {
                x[k] = x[k]+1 ;                                        // 搜索下一个节点 
                while ( x[k] < n ) {
                    if ( !visit[x[k]] && arc[x[k-1]][x[k]] ) {         // 这个节点没被访问过，且这个节点和上一个节点之间有通路
                        break ;
                    } 
                    else 
                        x[k] = x[k] + 1 ;                              // 不满足，尝试下一个节点 
                }
                if ( x[k] < n && k == n-1 && arc[x[k]][0] == 1 ) {     // 已经形成哈密顿回路了，输出结果  
                    showCircle() ; 
                    return ; 
                }
                if ( x[k] < n && k < n - 1 ) {                         // 满足哈密顿回路的部分解，尝试下一个位置，k++ 
                    visit[x[k]] = true ; 
                    k = k + 1 ; 
                }
                else {                                                 // 不满足哈密顿回路的部分解，回溯，取消对x[k]的访问设置，k--回到上一个位置
                    visit[x[k]] = false ;  
                    x[k] = 0 ; 
                    k = k - 1 ; 
                }
            }
        }
} ; 

int main() {
    Hamilton hc ; 
    hc.HamiltonCircle() ; 
    return 0 ;  
}
