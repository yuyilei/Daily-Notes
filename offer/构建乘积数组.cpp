/*
给定一个数组A[0,1,...,n-1],请构建一个数组B[0,1,...,n-1],其中B中的元素B[i]=A[0]*A[1]*...*A[i-1]*A[i+1]*...*A[n-1]。不能使用除法。
*/

class Solution {
public:
    vector<int> multiply(const vector<int>& A) {
        int len = A.size();
        vector<int> res (len, 0);
        if ( A.empty() )
            return res;
        res[0] = 1;
        // 此时 res[i] = A[0]*A[1]*...*A[i-1]
        for ( int i = 1 ; i < len ; i++ ) {
            res[i] = res[i-1] * A[i-1];
        }
        int tmp = 1;
        // 对于每个res[i] 在乘以 A[i+1]*...*A[n]
        for ( int i = len - 2 ; i >= 0 ; i-- ) {
            tmp *= A[i+1];
            res[i] *= tmp;
        }
        return res;
    }
};
