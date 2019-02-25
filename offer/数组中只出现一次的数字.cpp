/*
一个整型数组里除了两个数字之外，其他的数字都出现了偶数次。请写程序找出这两个只出现一次的数字。
*/

class Solution {
public:
    void FindNumsAppearOnce(vector<int> data,int* num1,int *num2) {
        int len = data.size();
        if( len <= 2 )
            return;
        int all = 0;
        // 找到最低位的不同 
        int last_pos = 1;
        (*num1) = (*num2) = 0;
        for ( int i = 0 ; i < len ; i++ ) {
            all ^= data[i];
        }
        while ( (all & last_pos) == 0 ) {
            last_pos <<= 1;
        }
        for ( int i = 0 ; i < len ; i++ ) {
            if ( (data[i] & last_pos) )
                (*num1) ^= data[i];
            else
                (*num2) ^= data[i];
        }
    }
};
