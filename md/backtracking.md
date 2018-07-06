# 回溯法 (backtracking)

回溯法（探索与回溯法）是一种基于深度优先的搜索法，又称为试探法，按选优条件向前搜索，以达到目标。但当探索到某一步时，发现原先选择并不优或达不到目标，就退回一步重新选择，即走不通就退。 

example: [回溯生成迷宫与解迷宫](https://github.com/yuyilei/Python_Homework/blob/master/maze.py)

**基本思想**：

回溯法是一种穷举类型的算法，回溯法是有组织的进行穷举，在试探过程中不断通过题设要求减少搜索空间，这种减少是是对搜索空间进行大规模剪枝，从而使得实际搜索空间远远小于问题的解空间，所以回溯法的实际运行效率还是比较高的。

**时间性能**：


1. 从n个元素的集合S中找出S满足某种性质的子集(不考虑顺序)，相应的解空间树称为子集树，如0/1背包问题。 这类子集树通常有2^n个叶结点，其结点总个数为2^(n+1)-1。遍历子集树的算法需Ω(2^n)计算时间。

2. 从n个元素满足某种性质的排列(考虑顺序)，相应的解空间树称为排列树，这类排列树通常有n!个叶结点。遍历排列树的算法需Ω(n!)计算时间。 

回溯法实际上属于蛮力穷举法，不能期望它有很好的最坏时间性能，在最坏的情况下（一点都没有进行剪枝），它的时间复杂度是O(2^n)或O(n!)，达到了指数或阶乘。但是在一般情况下，都能进行很好的剪枝，所以，回溯法一般具有较好的平均时间复杂度。 

**算法框架**：

递归法回溯法搜索子集树：


```c
    void Backtrack(int t)
    {
        if(t>n)
            Output(x) ;
        else
            for(i=0;i<=1;i++)
            {
                x[t]=i ;
                if( check(t) )               // 进行剪枝
                    Backtrack(t+1) ;
            }
    }
```

[递归法求N皇后](https://github.com/yuyilei/Algorithms/blob/master/C%2B%2B/recursive-Nqueue.cpp)

[递归法求数独](https://github.com/yuyilei/LeetCode/blob/master/C%2B%2B/037_Sudoku_Solver.cpp)

递归法回溯法搜索排列树：

```c
    void Backtrack(int t)
    {
        if(t>n)
            Output(x) ;
        else
            for(i=t;i<=n;i++)                // 考虑顺序， 0到t-1的部分被减去，相当于隐蔽的剪枝 
            {
                swap(x[t],x[i]) ;            //swap作用是交换两个元素
                if(check(t))                 // 进行剪枝 
                    Backtrack(t+1) ;
                swap(x[t],x[i]) ;
            }
    }
```


非递归法回溯：


```c
    void Backtrack() {
        int a[n] ;                                       // 解向量
        初始化a[0] ～ a[n-1]  
        int i = 1 ;      
        while ( i >= 1 && (尚未达到目标) ) {               // 还能回溯
            if ( i > n ) {                               // 已经是一组完整的解 
                得到一组解，输出 ; 
            } 
            else {
                a[i] = 第一个有可能的值 ; 
                while ( a[i]在不满足约束条件且在搜索空间内 ）{ // check函数
                    a[i] = 下一个有可能的值 ; 
                }
                if ( a[i]在搜索空间内 ) {                  // 是可行解
                    标记占用的资源 ;
                    i++ ;                                // 拓展下一个节点 
                } 
                else {                                   // 不是可行解 
                    清空占用的状态 ; 
                    i-- ;                                // 回溯 
                }
            }
        }
    }
```

[非递归求哈密顿回路](https://github.com/yuyilei/Algorithms/blob/master/C++/Hamilton.cpp)
