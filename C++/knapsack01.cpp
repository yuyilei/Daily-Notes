#include<cstdio>
#include<iostream>
#include<algorithm>
using namespace std ; 

/*
动态规划：0/1背包问题
0/1背包问题满足最优性原理，所以可以用动态规划。
v[i] 第 i 件的物品的价值。
w[i] 第 i 件的物品的重量。 
设 
    c[i][j] = 前i件物品在剩余容量为j的背包中能装下的最大价值
动态规划函数：
    c[i][j] = 0                                        //  i = 0 或 j = 0 
    c[i][j] = c[i-1][j]                                //  j < w[i] 装不下第i件物品
    c[i][j] = max(c[i-1][j] , c[i-1][j-w[i]]+v[i])     //  j >= w[i] 装得下第i件物品，就要选择装不装这件物品  

测试样例：
    输入：
    5
    10
    2 2 6 5 4
    6 3 5 4 6
    输出：
    背包能取得的最大价值是: 15
    包含的元素: (质量， 价值) :
    ( 2, 6 )
    ( 2, 3 )
    ( 4, 6 )
*/

class knapsack {
    private :
        int *w, *v ;                          // 分别是重量和价值    
        int n ;                               // 元素个数  
        int **c ;                             // 决策序列
        int cap ;                             // 容器体积 
        int *res ;                            // 决策结果  
    public : 
    knapsack() {
        cout << "输入元素个数：" << endl ; 
        cin >> n ; 
        cout << "输入容器体积" << endl ;  
        cin >> cap ; 
        w = new int [n+1] ; 
        v = new int [n+1] ; 
        res = new int [n+1] ; 
        c = new int* [n+1] ; 
        for ( int i = 0 ; i <= n  ; i++ ) 
            c[i] = new int [cap+1] ; 
        cout << "输入各个元素的体积" << endl ; 
        for ( int i = 1 ; i <= n ; i++ ) 
            cin >> w[i] ; 
        cout << "输入各个元素的价值" << endl ; 
        for ( int i = 1 ; i <= n ; i++ ) 
            cin >> v[i] ; 
    }
    void KnapSack() {
        int i, j, k, t ; 
        for ( i = 0 ; i <= n ; i++ ) 
            c[i][0] = 0 ; 
        for ( j = 0 ; j <= cap ; j++ ) 
            c[0][j] = 0 ; 
        for ( i = 1 ; i <= n ; i++ ) {
            for ( j = 1 ; j <= cap ; j++ ) {
                if ( w[i] > j ) {
                    c[i][j] = c[i-1][j] ; 
                } 
                else {
                    c[i][j] = max(c[i-1][j], c[i-1][j-w[i]]+v[i]) ; 
                }
            } 
        }
        for ( j = cap, i = n ; i > 0 && j >= 0 ; i-- ) {
            if ( c[i][j] > c[i-1][j] ) {
                res[i] = 1 ; 
                j -= w[i] ; 
            }
            else 
                res[i] = 0 ; 
        }
    }
    
    void showres() {
        int i ; 
        cout << "背包能取得的最大价值是: " << c[n][cap] << endl ; 
        cout << "包含的元素: (质量， 价值) :" << endl ; 
        for ( i = 1 ; i <= n ; i++ ) {
            if ( res[i] == 1 ) 
                printf("( %d, %d ) \n",w[i],v[i]) ; 
        }
    } 
} ; 

int main() {
    knapsack k ; 
    k.KnapSack() ; 
    k.showres() ; 
    return 0 ; 
}