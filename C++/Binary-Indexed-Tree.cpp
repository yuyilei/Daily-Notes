#include<cstdio>
using namespace std ; 

class BITree {
    private : 

    int *tree, size ; 
    int lowbit( int x) {
        return x & (-x) ; 
    } 
    public : 

    BITree(int s) : size(s+1) {                                            // 初始化tree数组
        tree = new int [s+1] ; 
        for ( int i = 0 ; i <= s ; i++ ) 
            tree[i] = 0 ;  
    } 

    int SumToRight(int right) {                                           // 求和 [1,right] 
        int res = 0 ; 
        for ( int i = right ; i >= 1 ; i -= lowbit(i) ) 
            res += tree[i] ; 
        return res ; 
    }

    int SumSection(int left, int right) {                                 // 求和 [left,right] 
        return SumToRight(right) - SumToRight(left-1) ; 
    }

    void update_node(int index, int value) {                              // 单点更新 
        for ( int i = index ; i < size ; i += lowbit(i) ) 
            tree[i] += value ; 
    }
    ~BITree() {
        delete [] tree ;  
    }
} ;  


int main() {
    // test demo .... 
    BITree bit(100) ; 
    int i, j, t ;
    for ( i = 1 ; i <= 100 ; i++ ) {                // 单点更新 
        bit.update_node(i,i) ;                      // tree[i] = i ;                        
    }
    printf("left = 1, right = 10, sum = %d\n",bit.SumSection(1,10)) ;             // 区间求和  
    printf("left = 5, right = 10, sum = %d\n",bit.SumSection(5,10)) ; 
    printf("left = 1, right = 17, sum = %d\n",bit.SumSection(1,17)) ; 
    printf("left = 18, right = 99, sum = %d\n",bit.SumSection(18,99)) ; 
    return 0 ; 
}