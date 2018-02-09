#include<iostream>
#include<cstring>
#include<cstdio> 
using namespace std ; 

// 用Dijkstra算法获取最短路径 　 
// 用临界矩阵储存图 
// 用于记录到每个点的最短路径 

/*
    测试数据： 
        6 8   
        0 4 30
        0 5 100
        1 2 5
        2 3 50
        3 5 10
        4 5 60
        4 3 20
        0 2 10
        0 
*/

struct Dis {
    string path ;                       // 途经路径 
    int value ;                         // 路径长度 
    bool visit ;                        // 标志位，表示此点是否已经获得了最短路径 
    Dis() {
        visit = false ;
        value = 0 ;
        path = "" ;
    }
} ;

struct Graph {
    int start ; 
    int vexnum ;   //图的顶点个数
    int edge ;     //图的边数
    int **arc ;   //邻接矩阵
    Dis *dis ;   //记录各个顶点最短路径的信息
} ; 

int main() {
    int i, j, a, b, c, count = 0 ;
    cout << "输入图的顶点个数，和图的边数:" << endl ; 
    struct Graph graph ; 
    cin >> graph.vexnum >> graph.edge ; 
    graph.dis = new Dis[graph.vexnum] ; 
    graph.arc = new int *[graph.vexnum] ; 
    for ( i = 0 ; i < graph.vexnum ; i++ ) 
        graph.arc[i] = new int [graph.vexnum] ; 
    cout << "输入每条边的顶点和终点及权重(顶点从0开始):" << endl ;  
    // 输入临界矩阵 
    for ( i = 0 ; i < graph.edge ; i++ ) {
        cin >> a >> b >> c ; 
        graph.arc[a][b] = c ;  
        // 如果是无向图，加上下面这行  
        // graph.arc[b][a] = c ;    
    } 
    
    // 输出图的临界矩阵 
    cout << "\n      临界矩阵:" << endl ; 
    for ( i = 0 ; i < graph.vexnum ; i++ ) {
        for ( j = 0 ; j < graph.vexnum ; j++ ) 
            printf("%4d ",graph.arc[i][j]) ; 
        cout << endl ; 
    }
    cout << "\n\n输入起点下标:" << endl ; 
    cin >> graph.start ; 

   // 初始化每个点的最短路径 
    for ( i = 0 ; i < graph.vexnum ; i++ ) {
        graph.dis[i].visit = false ; 
        graph.dis[i].value = (graph.arc[graph.start][i] == 0 ) ? INT8_MAX : graph.arc[graph.start][i] ; 
        graph.dis[i].path = "v" + to_string(graph.start) + " -->v" + to_string(i) ; 
    }
    // 把起始点加入集合  
    graph.dis[graph.start].visit = true ;  
    graph.dis[graph.start].value = 0 ; 
    
    // count表示集合中的点数 
    count = 1 ;  
    while ( count < graph.vexnum ) {
        int now = 0 ;                  // now 表示dis数组中最小的点的下标，也就是当前点 
        int minn = INT8_MAX ;  
        for ( i = 0 ; i < graph.vexnum ; i++ ) {
            if ( !graph.dis[i].visit && graph.dis[i].value < minn ) {
                now = i ;
                minn = graph.dis[i].value ; 
            }
        }
        // 把now对应的点放入集合中 
        graph.dis[now].visit = true ; 
        count++ ; 
        for ( i = 0 ; i < graph.vexnum ; i++ ) {
            // 从新加入集合的点向外找能够缩短的路径 
            if ( !graph.dis[i].visit && graph.arc[now][i] != 0 && graph.dis[now].value + graph.arc[now][i] < graph.dis[i].value) { 
                graph.dis[i].value = graph.dis[now].value + graph.arc[now][i] ; 
                graph.dis[i].path = graph.dis[now].path + " -->v" + to_string(i) ; 
           } 
        }
    }

    cout << "以v" << graph.start << "为起点的的最短路径为：" << endl ; 
    for ( i = 0 ; i < graph.vexnum ; i++ ){
        if ( graph.dis[i].value == INT8_MAX ) 
            cout << "v" << graph.start << " 到v" << i << "没有最短路径" << endl ; 
        else 
            cout << graph.dis[i].path << " = " << graph.dis[i].value << endl ; 
    }
    return 0 ; 
}
