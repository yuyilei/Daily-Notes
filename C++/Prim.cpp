#include<cstdio>
#include<algorithm>
using namespace std ;

/*
Prim算法（贪心算法）最小生成树
1. 选择一个起始点w，将w加入最小生成树的集合s
2. 在未加入点中选择一个离s的距离最小的点n，将n加入集合s，更新各个点到集合s的距离
3. 重复2，直到所有的都加入了s中 

时间复杂度: O(n^2) 

测试样例:
输入:
    6
    0 6 1 5 0 0
    6 0 5 0 3 0
    1 5 0 5 6 4
    5 0 5 0 0 2
    0 3 6 0 0 6
    0 0 4 2 6 0
输出:
    0 <--> 2 1
    2 <--> 5 4
    5 <--> 3 2
    2 <--> 1 5
    1 <--> 4 3 
*/

class MinSpanTree {
    private :
        struct minedge {
            int lowcost ;           // 最短路径
            int adjvex ;            // 邻接点 
            bool visit ; 
        } ; 
        int n ;                    // 顶点个数 
        int **arc ;                // 邻接矩阵
        int s ;                // 起始点，默认为1 
        struct minedge *e ;       // 最小边的数组 
    public :
        MinSpanTree() {
            printf("输入顶点的个数:\n") ; 
            scanf("%d",&n) ; 
            arc = new int* [n] ; 
            for ( int i = 0 ; i < n ; i++ ) {
                arc[i] = new int [n] ; 
            }
            int t ; 
            printf("输入邻接矩阵\n") ; 
            for ( int i = 0 ; i < n ; i++ ) {
                for ( int j = 0 ; j < n ; j++ ) {
                    scanf("%d",&t) ; 
                    t = ( t == 0 ) ? 99999 : t ;                     // 没有路就设为最大值  
                    arc[i][j] = t ; 
                }
            }
        }
        void Prim() {
            e = new struct minedge [n] ; 
            int i, j, k ; 
            s = 0 ; 
            for ( i = 0 ; i < n ; i++ ) {           // 初始化 
                e[i].adjvex = s ; 
                e[i].lowcost = arc[s][i] ; 
                e[i].visit = false ; 
            }
            e[s].visit = true ; 
            for ( i = 0 ; i < n - 1 ; i++ ) {
                int minn = 99999 ; 
                for ( j = 0 ; j < n ; j++ ) {
                    if ( !e[j].visit && e[j].lowcost > 0 && e[j].lowcost < minn ) {
                        minn = e[j].lowcost ; 
                        k = j ;  
                    }
                } 
                printf("%d <--> %d %d\n",e[k].adjvex,k,minn) ; 
                e[k].visit = true ; 
                for ( j = 0 ; j < n ; j++ ) {
                    if ( !e[j].visit && arc[j][k] < e[j].lowcost)  {
                        e[j].lowcost = arc[j][k] ; 
                        e[j].adjvex = k ; 
                    }
                }
            }
        } 
} ; 

int main() {
    MinSpanTree p ; 
    p.Prim() ; 
    return 0 ; 
}