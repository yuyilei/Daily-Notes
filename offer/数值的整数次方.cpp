/*
给定一个double类型的浮点数base和int类型的整数exponent。求base的exponent次方。
 */

/*
要注意 exponent 小于0和等于0的情况
*/
class Solution {
public:
    double Power(double base, int exponent) {
        if ( exponent == 0 )
            return 1;
        else if ( exponent < 0 )
            return 1.0 / Power(base, -exponent);
        else {
            double half = Power(base, exponent/2);
            if ( exponent % 2 )
                return half * half * base;
            else
                return half * half;
        }
    }
};
