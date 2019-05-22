/*
求出1~13的整数中1出现的次数,并算出100~1300的整数中1出现的次数？为此他特别数了一下1~13中包含1的数字有1、10、11、12、13因此共出现6次,但是对于后面问题他就没辙了。ACMer希望你们帮帮他,并把问题更加普遍化,可以很快的求出任意非负整数区间中1出现的次数（从1 到 n 中1出现的次数）。
 */

class Solution {
public:
    int NumberOf1Between1AndN_Solution(int n) {
        int res = 0;
        int pos = 1;
        while ( (n/pos) != 0 ) {
            int now = (n/pos) % 10;         // 当前位
            int high = n/(pos*10);          // 高位
            int low = n - (n/pos)*pos;      // 低位 
            if ( now == 0 ) {               // 当前位为0，1的个数由高位决定
                res += high*pos;
            }
            else if ( now == 1 ) {          // 当前位为1，1的个数由高位和低位决定
                res += high*pos + low+1;
            }
            else {                          // 当前位大于1，1的个数由高位决定
                res += (high+1)*pos; 
            }
            pos *= 10;
        }
        return res;
    }
};
