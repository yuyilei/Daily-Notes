#include<cstdio>
using namespace std ;

/*
减治法：
俄式乘法 
对两个正整数 n*m 可以化为 
1. 如果 n % 2 == 0, n * m = (n/2) * (2*m) 
2. 如果 n % 2 == 1, n * m = ((n-1)/2) * (2*m) + m 
*/ 

/*
如果一个函数中所有递归形式的调用都出现在函数的末尾，这个递归函数是尾递归的。
当递归调用是整个函数体中最后执行的语句且它的返回值不属于表达式的一部分时，这个递归调用就是尾递归。
尾递归函数的特点是在回归过程中不用做任何操作。
*/ 

// 尾递归法： 
void multi1(int n, int m, int &res) {
   // printf("n: %d m: %d res: %d\n",n,m,res) ; 
    if ( n == 1 ) {
        res += n*m ; 
        return ; 
    } 
    if ( n % 2 == 0 )
        return multi1(n/2,2*m,res) ; 
    else  {
        res = res + m ;                        // 引用传值要注意 ，引用的传值不能传表达式。 
        return multi1((n-1)/2,2*m,res) ; 
    } 
}

// 迭代法 ：
void multi2(int n, int m, int & res) {
    while( n > 0 ) {
        if ( n % 2 == 0 ) {
            n /= 2 ; 
            m *= 2 ; 
        } 
        else {
            res += m ; 
            m *= 2 ; 
            n = ( n-1 )/2 ; 
        }
    }
}

int main() {
    int res1 = 0, res2 = 0 ;
    multi1(50,65,res1) ;
    printf("%d\n",res1) ; 
    multi2(50,65,res2) ; 
    printf("%d\n",res2) ; 
    return 0 ; 
}