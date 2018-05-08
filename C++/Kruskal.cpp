#include<cstdio>
#include<algorithm> 
#include<queue> 
using namespace std ;

/*
Kruskal算法(贪心算法)，求最小生成树
每次查找权重最小的边，直到这个图只有一个连通分量 
利用并查集来检查是不是一个连通分支
适用于稀疏图
时间复杂度与边有关 
时间复杂度：O(e*log(e))   e为边的个数，
O(e*log(e))为排序的时间复杂度，O(n)为并查集的时间复杂度
测试样例1:
输入：
    10 6
    1 2 6
    1 3 1
    1 4 5
    2 3 5
    2 5 3
    3 5 6
    3 6 4
    4 3 5
    4 6 2
    5 6 6
输出：
    1 <--> 3  1
    4 <--> 6  2
    2 <--> 5  3
    3 <--> 6  4
    2 <--> 3  5

测试样例2:
输入：
    7 9
    1 2 20
    1 5 1
    2 3 6
    2 4 4
    3 7 2
    4 6 12
    4 7 8
    5 6 15
    6 7 10
输出：
    1 <--> 5  1
    3 <--> 7  2
    2 <--> 4  4
    2 <--> 3  6
    6 <--> 7  10
    5 <--> 6  15 
*/

class MinSpanTree {
    private :
        struct edge {
            int x, y, w ;          // 分别是边的两个顶点和权重  
            edge(int a, int b, int c) : x(a), y(b), w(c) {}
            edge() {}
        } ; 

        // 不知道为啥这个cmp函数要用static方法诶。。。 
        static bool cmp(struct edge a, struct edge b) {
            return a.w < b.w ;    //  按权重排序 
        }

        int n ;                    // 边的个数 
        int d ;                    // 点的个数， 点从1到d 
        struct edge * e ;           // 储存边 
        int *p ;                   // 并查集的辅助数组 

        int root(int a) {
            if ( a != p[a] ) 
                p[a] = root(p[a]) ;              // 查找根节点，并压缩路径，尽量缩短树的高度，加快查找时间 
            return p[a] ;           
        }

        bool connected(int a, int b) {
            return root(a) == root(b) ; 
        }
        void Union(int a, int b) {
            if ( !connected(a,b) )                
                p[root(a)] = root(b) ; 
        }

    public : 
        MinSpanTree() {
            printf("输入边的个数，点的个数:\n") ; 
            scanf("%d %d",&n,&d) ; 
            p = new int [d+1] ; 
            e = new struct edge [n] ; 
            printf("分别输入所有的边的定点，权重:\n") ; 
            for ( int i = 0 ; i < n ; i++ ) {
                scanf("%d %d %d",&e[i].x,&e[i].y,&e[i].w) ;  
            }
            for ( int i = 1 ; i <= d ; i++ ) {
                p[i] = i ;               // 初始化并查集，使每个节点的根节点是自身 
            }
        }
        void Kruskal() {
            sort(e,e+n,cmp) ;             // 根据权重进行排序
            for ( int i = 0 ; i < n ; i++ ) {
                if ( !connected(e[i].x,e[i].y) ) {
                    Union(e[i].x,e[i].y) ; 
                    printf("%d <--> %d  %d\n",e[i].x,e[i].y,e[i].w) ; 
                }                
            } 
        }
} ; 

int main() {
    MinSpanTree k ; 
    k.Kruskal() ;  
    return 0 ; 
}
