/*
把只包含质因子2、3和5的数称作丑数（Ugly Number）。例如6、8都是丑数，但14不是，因为它包含质因子7。 习惯上我们把1当做是第一个丑数。求按从小到大的顺序的第N个丑数。
*/

class Solution {
public:
    int GetUglyNumber_Solution(int index) {
        if ( index <= 1 )
            return index;
        int i2 = 0, i3 = 0, i5 = 0;
        vector<int> res (index,0);
        res[0] = 1;
        for ( int i = 1 ; i < index ; i++ ) {
            res[i] = min(min(res[i2]*2, res[i3]*3), res[i5]*5);
            if ( res[i] == res[i2]*2 )
                i2++;
            // 不能是 else if
            if ( res[i] == res[i3]*3 )
                i3++;
            if ( res[i] == res[i5]*5 )
                i5++;
        }
        return res[index-1];
    }
};
