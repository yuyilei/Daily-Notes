#include<cstdio>
#include<iostream> 
using namespace std ; 
/*
动态规划：最优二叉查找树的建立
最优二叉查找树的性质：所有节点的平均查找长度最短，注意与哈夫曼树的区别(哈夫曼树是所有叶子节点的平均查找长度最短)
最优二查找叉树满足最优性原理：
如果T是最优最优二叉查找树的话，T的左右子树也一定是最优二叉查找树。
设 C[i][j] 是二叉查找树T[i][j]的 平均查找长度。p[i]是节点i处的频率 
则有：
    c[i][j] = min{c[i][k-1] + c[k+1][j] + (p[i]+...+p[j]) } （ i <= k <= j ） 

动态规划函数:
    c[i][i-1] = 0                          1 <= i <= n+1 
    c[i][i] = p[i-1]                       1 <= i <= n+1 
    c[i][j] = min{c[i][k-1] + c[k+1][j] + (p[i]+...+p[j]) } （ 1 <= i <= k <= j <= n+1 ）  
*/

class OptimalBST {
    private :
        float *p ;                   // 各个节点的频率
        int **r ;                    // 记录根结点 
        float **c ;                    // 存储二叉查找的平均次数
        int n ;                      // 二叉树的节点个数 
        float res ;                  // 存结果
    public :
    OptimalBST() {
        cout << "输入二叉树的节点个数:" << endl ; 
        cin >> n ; 
        p = new float [n+2] ; 
        r = new int* [n+2] ; 
        c = new float* [n+2] ; 
        for ( int i = 0 ; i < n+2 ; i ++ ) {
            c[i] = new float [n+2] ;   
            r[i] = new int [n+2] ; 
        }
        cout << "输入各个节点的频率" << endl ; 
        for ( int i = 0 ; i < n ; i++ ) {
            scanf("%f",&p[i]) ; 
        }
    }
    void getOptimal() {
        int i, j, k, d ; 
        for ( i = 1 ; i < n + 1 ; i ++ ) {
            c[i][i-1] = 0 ; 
            c[i][i] = p[i-1] ; 
            r[i][i] = i ; 
        }
        c[n+1][n] = 0 ; 
        for ( d = 1 ; d < n ; d++ ) {                                  // 从对角线开始向右上角填表 
            for ( i = 1 ; i <= n - d ; i++ ) {
                j = i + d ; 
                int tmpk = i ; 
                float sum = 0, minn = 99999 ; 
                for ( k = i ; k <= j ; k++ ) {
                    sum += p[k-1] ; 
                    if ( c[i][k-1] + c[k+1][j] < minn ) {
                        minn = c[i][k-1] + c[k+1][j] ; 
                        tmpk = k ; 
                    }
                }
                c[i][j] = minn + sum ; 
                r[i][j] = tmpk ;  
            } 
        } 
        res = c[1][n] ; 
    } 
    
    void showres() {
        cout << "最小平均查找长度:" ; 
        printf("%f\n",res) ; 
    }
} ; 

int main() {
    OptimalBST BST ; 
    BST.getOptimal() ;   
    BST.showres() ; 
}