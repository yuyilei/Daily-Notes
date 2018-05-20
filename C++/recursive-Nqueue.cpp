#include<cstdio>
#include<iostream>
#include<cmath> 
using namespace std ;

/*
递归求N皇后问题（回溯）
输入：皇后的个数N
输出：所有的解集 
设x[i] 为 第i个皇后的坐标（i从0开始）
*/

class Nqueue {
    private :
        int n ;                                 // 皇后的个数
        int *x ;                                // 记录放置皇后放置位置的数组 
        long res ;                               // 结果的数量 
    public : 
        Nqueue() {
            printf("输入皇后的个数(皇后个数小于等于14):\n") ; 
            scanf("%d",&n) ; 
            x = new int [n] ; 
            res = 0 ; 
        }
        
        bool place(int k) {                     // 检查皇后k在x[k]是否冲突  
            for ( int i = 0 ; i < k ; i++ ) {
                if ( x[i] == x[k] || abs(i-k) == abs(x[i]-x[k]) ) 
                    return false ; 
            } 
            return true ; 
        }

        void showQueue() {                      // 显示所有皇后的坐标 
            printf("(") ; 
            for ( int i = 0 ; i < n ; i++ ) {
                printf(" %d",x[i]) ; 
            }
            printf(" )\n") ; 
        }

        void queue(int row) {
            if ( row == n ) {
                showQueue() ; 
                res++ ; 
                return ; 
            }
            for ( int i = 0 ; i < n ; i++ ) {      // 对于第row行，尝试放置皇后    
                x[row] = i ;                       // 把第row行的皇后放在第i位
                if ( place(row) ) {                // 检查放置第row行的皇后之后，与之前的放置的皇后是否有冲突，如果没有冲突，就继续放置下一个皇后 
                    queue(row+1) ; 
                }
            }
        }

        void getres() {
            queue(0) ; 
            printf("%d皇后一共有%ld个解\n",n,res) ; 
        }
} ; 

int main() {
    Nqueue nq ; 
    nq.getres() ; 
    return 0 ; 
}