/*
输入n个整数，找出其中最小的K个数。例如输入4,5,1,6,2,7,3,8这8个数字，则最小的4个数字是1,2,3,4,。
*/

// 用最大堆保存这k个数，每次只和堆顶比，如果比堆顶小，删除堆顶，新数入堆。
class Solution {
public:
    vector<int> GetLeastNumbers_Solution(vector<int> input, int k) {
        vector<int> res;
        if ( input.empty() || k <= 0 || k > input.size() )
            return res;
        for ( int i = k/2 -1 ; i >= 0 ; i-- ) {
            adjustMaxHeapSort(input, i, k-1);
        }
        for  ( int i = k ; i < input.size() ; i++ ) {
            if ( input[i] < input[0] ) {
                input[0] = input[i];
                adjustMaxHeapSort(input, 0, k-1);
            }
        }
        for ( int i = 0 ; i < k ; i++ ) {
            res.push_back(input[i]);
        }
        return res;
    }
    void adjustMaxHeapSort(vector<int> &input, int pos, int len) {
        int tmp = input[pos];
        int child;
        for ( ; 2*pos+1 <= len ; pos = child ) {
            child = 2*pos+1;
            if ( child+1 <= len && input[child] < input[child+1] )
                child = child + 1;
            if ( input[child] > tmp )
                input[pos] = input[child];
            else
                break;
        }
        input[pos] = tmp;
    }
};
