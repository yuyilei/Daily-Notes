/*
 大家都知道斐波那契数列，现在要求输入一个整数n，请你输出斐波那契数列的第n项（从0开始，第0项为0）。
n<=39
 */

class Solution {
public:
    int Fibonacci(int n) {
        int a = 0, b = 1;
        for ( int i = 0 ; i < n ; i++ ) {
            int tmp = a + b;
            a = b;
            b = tmp;
        }
        return a;
    }
};

 
