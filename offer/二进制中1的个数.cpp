/*
输入一个整数，输出该数二进制表示中1的个数。其中负数用补码表示。
 */

/*
int的存储方式就是补码，只要去校验n的每一位是否为1即可
注意 flag 为 unsigned int
*/
class Solution {
public:
     int  NumberOf1(int n) {
         int res = 0;
         unsigned int flag = 1;
         while ( flag ) {
             if ( n & flag )
                 res++;
             flag <<= 1;
         }
         return res;
     }
};
