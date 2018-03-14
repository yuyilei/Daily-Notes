# KMP算法 (由Knuth，Morris，Pratt同时设计)

这是demo --> [( ๑´･ᴗ･`๑) ❤](https://github.com/yuyilei/Algorithms/blob/master/C%2B%2B/KMP.cpp)

字符串匹配有种暴力解法： 

    如果现在文本串S匹配到 i 位置，模式串T匹配到 j 位置 : 

    如果当前字符匹配成功（即S[i] == T[j]），则i++，j++，继续匹配下一个字符；
    
    如果失配（即S[i]! = T[j]），令i = i - (j - 1)，j = 0。相当于每次匹配失败时，i 回溯，j 被置为0。

伪代码如下： 

```
    int BF(char S[], char T[]) {
        int i, j ; 
        while ( S[i] != '\0' && T[j] != '\0' ) {
            if ( S[i] == T[j] ) {
                i++ ; 
                j++ ; 
            } 
            else {
                i = i - j + 1 ; 
                j = 0 ; 
            }
        } 
        if ( T[j] == '\0' ) 
            return index ; 
        return -1 ; 
    }
```
  

因为在匹配的过程中存在许多**不需要的回溯**，暴力算法的效率低。 

暴力算法在最坏情况的下时间复杂度为 `O(m*n)` 

KMP算法*是**暴力算法**与**next数组**的结合。 

KMP算法只对主串扫描一遍，时间复杂度为 `O(n)`, 但是增加了空间复杂度，next数组的空间复杂度为 `O(m)`

其中next数组是 每次S与T匹配不成功时，子串T的下标j回溯到的位置。 

```
    if ( S[i] != T[j] ) {
        j = next[j] ;  
    }
```

KMP算法的匹配过程： 

    1. 在串S和串T中中分别设置初始下标， i = 0, j = 0 ;

    2. 重复以下操作，直到S或T所有的字符比较完毕：

        2.1 如果 S[i] == T[j] ，继续比较S和T的下一对字符

        2.2 否则，j 回溯 ，j = next[j]，如果此时，j == -1 则， j++， i++ 

    3. 如果T中所有字符都比较完毕，则匹配成功，否则，失败 

KMP算法的代码如下： 

```
    int kmp(char S[], char T[], int next[]) {
        int i = 0, j = 0 ;
        while ( S[i] != '\0' && T[j] != '\0' ) {
            if ( S[i] == T[j] ) {
                i++ ; 
                j++ ; 
            } 
            else {
                j = next[j] ;                                                    // 匹配不成功 子串从 next[j] 开始 
                if ( j == -1 ) {                                                  
                    i++ ; 
                    j++ ; 
                }
            }
        }
        if ( T[j] == '\0' ) {
            return ( i - tlen ) ; 
        }
        return -1 ; 
    }
```

那么next数组是如何获取的呢？

对next[j]来说，next[j]的值只依赖于子串K本身，而与主串S无关。

对子串K来说： 

    真前缀：T[0] ~ T[k-1] 

    真后缀：T[j-k] ~ T[j-1] 

    k : 1 <= k < j 

定义next数组如下： 

    j = 0, next[j] = -1 ;

    j != 0, 且真前缀和真后缀相等的最大子串长度大于1，next[j] = 真前缀和真后缀相等的最大子串长度。 

    其他， next[j] = 0 

获取next数组： 

```
   void getNext(char S[], char T[], int next[] ) {
        int i, j, maxlen ; 
        ifnext = true ; 
        next[0] = -1 ; 
        for ( j = 1 ; T[j] != '\0' ; j++ ) {                                     // 依次求 next[j]
            for ( maxlen = j - 1 ; maxlen >= 1 ; maxlen-- ) {                    // 从next[j] 可能的最大值maxlen开始 ,求得maxlen的值
                for ( i = 0 ; i < maxlen ; i++ ) 
                    if ( T[i] != T[j-maxlen+i] ) 
                        break ; 
                if ( i == maxlen ) {                                              // next[j] 的值                                      
                    next[j] = maxlen ; 
                    break ; 
                }
            }
            if ( maxlen < 1 )                                                     
                next[j] = 0 ; 
        }
    }
```

获取next数组的优化：

上述方法获得next的方式，在最坏情况下，时间复杂度为 O(m^3) ，需要不断的比较真前缀和真后缀是否相等。其中模式的长度为m。

优化后的求next数组方法的时间复杂度为O(m)。 方法如下：

思路： 

1）next[0] = -1  (T[0]没有真前缀和真后缀)

2) next[1]到next[j] 都已经求出来了。

3) 求next[j+1] 

其实感觉就是数学归纳法。 

当next[j] 已知时，设 k = next[j], 则 T[0,...,k-1] == T[j-k,...,j-1]  

求next[j+1] 有两种情况： 

1） T[k] == T[j]，真前缀和真后缀相等(T[0,...k] == T[j-k,...,j])，而且这是最长的真前缀和真后缀相等。 则 next[j+1] = k+1

2） T[k] != T[j]，需要另外寻找新的最长和真前缀和真后缀相等的子串（即T[0,...,j-1]的第二长的相等真前缀和真后缀的长度。 

在第二种情况中，T[0,...,j-1]中第二长的相等的真前缀和真后缀 是 T[0,...,j-1] 中最长的相等的真前缀的子串的最长相等真前缀。 

证明如下： 

因为： T[0,...,k-1] == T[j-k,...,j-1] 

所以： T[k-next[k],...,k-1] == T[j-next[k],...,j-1] ( 里面的一个子串)

又因为： T[0,...,next[k]-1] == T[k-next[k],...,k-1] ( T[0,...,j-1] 中的最长真前缀子串) 

所以：  T[0,...next[k]-1] == T[j-next[k],...,j-1] 

所以，每次不匹配的时候，都令  k = next[k] 

代码如下： 

```
    void getNext(char T[], int next[]) {
        int k = -1, j = 0 ; 
        next[0] = -1 ; 
        while( T[j] != '\0' ) {
            if ( k == -1 ) {
                next[++j] = 0 ;                 // 结束这个点的查找，无相同子串  
            }
            else if ( T[j] == T[k] ) {
                next[++j] = k + 1 ;             // 结束这个点的查找，相同子串长度为 k+1  
            } 
            else 
                k = next[k] ; 
        }
    }
```