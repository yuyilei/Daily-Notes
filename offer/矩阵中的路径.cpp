/*
请设计一个函数，用来判断在一个矩阵中是否存在一条包含某字符串所有字符的路径。路径可以从矩阵中的任意一个格子开始，每一步可以在矩阵中向左，向右，向上，向下移动一个格子。如果一条路径经过了矩阵中的某一个格子，则之后不能再次进入这个格子。 例如 a b c e s f c s a d e e 这样的3 X 4 矩阵中包含一条字符串"bcced"的路径，但是矩阵中不包含"abcb"路径，因为字符串的第一个字符b占据了矩阵中的第一行第二个格子之后，路径不能再次进入该格子。
 */

class Solution {
public:
    bool hasPath(char* matrix, int rows, int cols, char* str) {
        if ( rows == 0 || cols == 0 )
            return false;
        vector<vector<char>> help (rows, vector<char> (cols, -1));
        int index = 0;
        for ( int i = 0 ; i < rows ; i++ ) {
            for ( int j = 0 ; j < cols ; j++ ) {
                help[i][j] = matrix[index++];
            }
        }
        string s(str);
        return HasPath(help, rows, cols, s);
    }
    bool HasPath(vector<vector<char>> &help, int rows, int cols, string s) {
        for ( int i = 0 ; i < rows ; i++ ) {
            for ( int j = 0 ; j < cols ; j++ ) {
                if ( HasEachPath(help, rows, cols, i, j, s, 0) )
                    return true;
            }
        }
        return false;
    }

    bool HasEachPath(vector<vector<char>> &help, int rows, int cols, int r, int c, string s, int index) {
        if ( r < 0 || r >= rows || c < 0 || c >= cols || help[r][c] == -1 )
            return false;
        if ( index >= s.length() )
            return false;
        if ( index == s.length()-1 ) {
            return s[index] == help[r][c];
        }
        if ( s[index] != help[r][c] )
            return false;
        char tmp = help[r][c];
        help[r][c] = -1;
        bool res = HasEachPath(help, rows, cols, r+1, c, s, index+1) ||
                   HasEachPath(help, rows, cols, r-1, c, s, index+1) ||
                   HasEachPath(help, rows, cols, r, c+1, s, index+1) ||
                   HasEachPath(help, rows, cols, r, c-1, s, index+1);
        help[r][c] = tmp;
        return res;
    }
};
