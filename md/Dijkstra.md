# Dijkstra算法 

这是用Dijkstra+bfs写的一道pat --> [٩̋(๑˃́ꇴ˂̀๑)۶](https://github.com/yuyilei/Pat_Advanced_Level/blob/master/Advanced_Pat/1111.cpp)

**最短路径** : 

 > 从图中的某个顶点出发到达另外一个顶点的所经过的边的权重和最小的一条路径，称为最短路径。 

 **Dijkstra**算法是一种求最短路径的方法，用于求没有**负权重**的图中的最短路径 

## 算法介绍

 **Dijkstra**算法使用了广度优先搜索解决赋权有向图或者无向图的单源最短路径问题，算法最终得到一个最短路径树。

 **Dijkstra**算法借助一个数组dis来保存源点到各个顶点的最短距离和一个集合S保存已经找到了最短路径的顶点。
 
 当图中的所有点都在S中时，表示所有的最短路径查找完成 

 步骤如下 ： 

 1） 在dis数组中找出最小值，则该值就是源点s到该值对应的顶点的最短路径，并且把该点加入到S中。 
 
 2） 新加入的顶点是否可以到达其他顶点并且看看通过该顶点到达其他点的路径长度是否比源点直接到达短，如果是，那么就替换这些顶点在dis中的值。 重复1）直到所有的点都在S内. 

## 算法demo 

```
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
```

完整代码 --> [Dijkstra算法](https://github.com/yuyilei/Algorithms/blob/master/C%2B%2B/Dijkstra.cpp)

演示结果：

```
输入图的顶点个数，和图的边数:
6 8
输入每条边的顶点和终点及权重(顶点从0开始):
0 4 30
0 5 100
1 2 5
2 3 50
3 5 10
4 5 60
4 3 20
0 2 10

      临界矩阵:
   0    0   10    0   30  100
   0    0    5    0    0    0
   0    0    0   50    0    0
   0    0    0    0    0   10
   0    0    0   20    0   60
   0    0    0    0    0    0


输入起点下标:
0
以v0为起点的的最短路径为：
v0 -->v0 = 0
v0 到v1没有最短路径
v0 -->v2 = 10
v0 -->v4 -->v3 = 50
v0 -->v4 = 30
v0 -->v4 -->v3 -->v5 = 60

```

