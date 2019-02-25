/*
统计一个数字在排序数组中出现的次数。
 */

class Solution {
public:
    int GetNumberOfK(vector<int> data ,int k) {
        int res = 0, index = -1;
        int left = 0, right = data.size()-1;
        while ( left <= right ) {
            int mid = (left+right)/2;
            if ( data[mid] == k ) {
                res = 1;
                index = mid;
                break;
            }
            else if ( data[mid] > k ) {
                right--;
            }
            else {
                left++;
            }
        }
        if ( index == -1 )
            return 0;
        for ( int i = index + 1 ; i < data.size() && data[i] == k ; i++ )
            res++;
        for ( int i = index - 1 ; i >= 0 && data[i] == k ; i-- )
            res++;
        return res;
    }
};
