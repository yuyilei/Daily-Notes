/*
一只青蛙一次可以跳上1级台阶，也可以跳上2级……它也可以跳上n级。求该青蛙跳上一个n级的台阶总共有多少种跳法。
 */

/*
 f(n)=f(n-1)+f(n-2)+...+f(1)+1
 f(n-1)=f(n-2)+f(n-3)+...+f(1)+1
 所以 f(n)=2*f(n-1)
 */
class Solution {
public:
    int jumpFloorII(int number) {
        if ( number < 1 )
            return 0;
        int res = 1;
        for ( int i = 1 ; i < number ; i++ ) {
            res *= 2;
        }
        return res;
    }
};

// 位运算 
class Solution {
public:
    int jumpFloorII(int number) {
        if ( number < 1 ) 
            return 0;
        return 1 << (number-1);
    }
};
