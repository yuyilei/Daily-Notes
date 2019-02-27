/*
输入一个矩阵，按照从外向里以顺时针的顺序依次打印出每一个数字，例如，如果输入如下4 X 4矩阵： 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 则依次打印出数字1,2,3,4,8,12,16,15,14,13,9,5,6,7,11,10.
 */


class Solution {
public:
    vector<int> printMatrix(vector<vector<int> > matrix) {
        vector<int> res;
        if ( matrix.empty() || matrix[0].empty() )
            return res;
        int m = matrix.size(), n = matrix[0].size();
        // 行列刚开始需要遍历的长度分别是m-1, n 所以 下面有个 m-- 
        int all = m*n;
        int i = 0, j = 0;
        m--;
        while ( res.size() < all ) {
            for ( int k = 0 ; k < n ; k++ )
                res.push_back(matrix[i][j++]);
            if ( res.size() >= all )
                break;
            n--;
            i++;
            j--;
            for ( int k = 0 ; k < m ; k++ )
                res.push_back(matrix[i++][j]);
            if ( res.size() >= all )
                break;
            m--;
            i--;
            j--;
            for ( int k = 0 ; k < n ; k++ )
                res.push_back(matrix[i][j--]);
            if ( res.size() >= all )
                break;
            n--;
            i--;
            j++;
            for ( int k = 0 ; k < m ; k++ )
                res.push_back(matrix[i--][j]);
            if ( res.size() >= all )
                break;
            m--;
            i++;
            j++;
        }
        return res;
    }
};
