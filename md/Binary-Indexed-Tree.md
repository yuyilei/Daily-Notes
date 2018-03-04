# 树状数组 (Binary Indexed Tree) 

树状数组(Binary Indexed Tree(B.I.T), Fenwick Tree)是一个查询和修改复杂度都为log(n)的数据结构。主要用于查询任意两位之间的所有元素之和。 

(∪´>‿<`)   --> [这是demo](https://github.com/yuyilei/Algorithms/blob/master/C%2B%2B/Binary-Indexed-Tree.cpp) 

之前有个 PAT甲级也用了它！ --> [(☆▽☆)y](https://github.com/yuyilei/Pat_Advanced_Level/blob/master/Advanced_Pat/1057.cpp) 

## 树状数组定义 

初始化数组如下： 


```
    int source[9] ;                    //  原数组 
    int tree[9] ;                      //  树状数组 
```

> 注意：树状数组的下标从1开始，所以我们定义source的下标也从1开始 。 

如图： 

![](http://ohr9krjig.bkt.clouddn.com/Screen%20Shot%202018-03-04%20at%2015.23.18.png)

其中数组A就是原数组source，数组C就是树状数组tree。

可以看到 

```
    tree[i] = { 以tree[i]为根节点的树的叶子结点的权值之和 } 
```

树状数组的举例如下 ：

```
    tree[1] = source[1] ; 
    tree[2] = source[1] + source[2] ; 
    tree[3] = source[3] ; 
    tree[4] = source[1] + source[2] + source[3] + source[4] ; 
    tree[5] = source[5] ;
    tree[6] = source[5] + source[6] ; 
    tree[7] = source[7] ; 
    tree[8] = source[1] + source[2] + source[3] + source[4] + source[5] + source[6] + source[7] + source[8] ;  
``` 

将 **tree** 的下标换成二进制，如下图 : 

```
    0001    tree[1] = source[1] ; 
    0010    tree[2] = source[1] + source[2] ; 
    0011    tree[3] = source[3] ; 
    0100    tree[4] = source[1] + source[2] + source[3] + source[4] ; 
    0101    tree[5] = source[5] ;
    0110    tree[6] = source[5] + source[6] ; 
    0111    tree[7] = source[7] ; 
    1000    tree[8] = source[1] + source[2] + source[3] + source[4] + source[5] + source[6] + source[7] + source[8] ;  
``` 

可以发现: 

```
    tree[i] = source[i-2^k+1] + source[i-2^k+2]+......source[i] ; 
    // k为i的二进制数中，最低位的1的位数 ，如 i = 8 时，k = 3  
```

引入 **lowbit(int i)** : 

```
    int lowbit(int i) {
        return i & (-i) ;     // 获取最低位的1   lowbit(x) = 2^k （k为x的二进制数中，最低位的1的位数)  
    }
```

所以，得到**tree**的定义 :

```
    tree[i] = source[i] + source[i-1]+......+ source[i-lowbit(i)+2] + source[i-lowbit(i)+1] ;
```

## 树状数组的应用

### 区间求和

利用树状数组，能求出原数组source在某一区间内的和。 

定义**sum**： 

```
    sum[i] = source[1] + source[2] + ... + source[i] ; 
```

所以source在[left,right] 之间的和等于 ： `sum[right] - sum[left-1]`

由此可见，求区间和就是求source数组的前缀和。 

以sum[7]为例：

```
    sum[7] = source[1] + ... + source[7] ; 
    tree[7] = source[7] ; 
    tree[6] = source[5] + source[6] ;
    tree[4] = source[1] + source[2] + source[3] + source[4] ; 
```

所以 ：

```
    sum[7] = tree[7] + tree[6] + tree[4] ; 
```

将下标换成二进制： 

```
    sum[111] = tree[111] + tree[110] + tree[100] ;
```

观察可得： 

```
    sum[index] = 0 ;
    for ( int i = index ; i > 0 ; i -= lowbit(i) ) 
        sum[index] += source[i] ; 
```

区间求和： 

```
    int values(int left, int right) {
        return sum[right] - sum[left-1] ; 
    }
```

### 单点更新 

如果source数组中的某个值被修改了，要对tree进行更新： 

如果source[1] 修改了， 需要更新 tree[1], tree[2], tree[4], tree[8] 

如果source[2] 修改了， 需要更新 tree[2], tree[4], tree[8] 

如果source[3] 修改了， 需要更新 tree[3], tree[4], tree[8] 

把下标都改为为二进制： 

    (0001)      --更新-->  (0001) (0010) (0100) (1000) 
    (0010)      --更新-->  (0010) (0100) (1000)
    (0011)      --更新-->  (0011) (0100) (1000) 

观察可得 : 

```
    void update_node(int index, value) {
        for ( int i = index ; i <= size ; i += lowbit(i) ) 
            tree[i] += value ; 
    }
```

可以单点更新是区间求和的逆过程。 

