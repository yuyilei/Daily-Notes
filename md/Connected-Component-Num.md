# 连通分量的个数 （并查集的应用） 

无向图的极大连通子图成为**连通分量** ，现在给出一个**无向图**，要求它的连通分量，以及任意两点是否属于同一个连通分量。 

其实就是**并查集**的应用。 

### 原理 

设置每一个连通分量有一个根节点，这个连通分量的中所有的节点的前驱节点（或者父节点）都是这个根节点，同理，这个根节点的前驱节点是自己。 

所以，连通分量的个数就是所有前驱节点等于自身的节点的个数。

每两个节点之间有连接时，这两个节点所属的连通分量合并为同一个连通分量。 

### demo 

demo的链接 --> [并查集的应用](https://github.com/yuyilei/Algorithms/blob/master/C%2B%2B/Union.cpp)

合并a,b两点（其实就是合并a,b所属的连通分量为同一个连同分量 

```
void Union(int a, int b) {                                 
    int x = root(a) ;                          // 查看a的根节点
    int y = root(b) ;                          // 查看b的根节点  
    if ( father[x] != father[y] )              // 如果两者的根节点不同，就把x的根节点设为y，意为合并两点
        father[x] = y ;  
} 
```

查找根节点，并缩短路径，缩短路径是为了降低树的高度，加快查找速度 

```
int root(int a) {
    if ( a != father[a] ) 
        father[a] = root(father[a]) ;          // 查找根节点，并压缩路径
    return father[a] ;   
}
```

判断连通分支的个数

```
int Component(int father[], int n) {
    int res = 0 ; 
    for ( int i = 0 ; i < n ; i++ ) 
        if ( father[i] == i )                  // 查找所有节点，父节点（先驱节点时自己的就是一个连通分量的根节点   
            res ++ ; 
    return res ; 
}
```

判断两个点是否属于同一个连通分量

```
bool tell_Component(int a,int b ) {
    return root(a) == root(b) ;                // 根节点相同就是同一个连通分量 
}
```
