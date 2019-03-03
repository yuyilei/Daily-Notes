/*
地上有一个m行和n列的方格。一个机器人从坐标0,0的格子开始移动，每一次只能向左，右，上，下四个方向移动一格，但是不能进入行坐标和列坐标的数位之和大于k的格子。 例如，当k为18时，机器人能够进入方格（35,37），因为3+5+3+7 = 18。但是，它不能进入方格（35,38），因为3+5+3+8 = 19。请问该机器人能够达到多少个格子？
*/

public:
    int movingCount(int threshold, int rows, int cols) {
        vector<vector<bool>> help(rows, vector<bool> (cols, false));
        return countmoving(help, rows, cols, 0, 0, threshold);
    }
    int countmoving(vector<vector<bool>> &help, int rows, int cols, int x, int y, int k) {
        if ( x < 0 || x >= rows || y < 0 || y >= cols || help[x][y] ||
               get_sum(x) + get_sum(y) > k)
            return 0;
        help[x][y] = true;
        return 1 + countmoving(help, rows, cols, x+1, y, k)
                 + countmoving(help, rows, cols, x-1, y, k)
                 + countmoving(help, rows, cols, x, y+1, k)
                 + countmoving(help, rows, cols, x, y-1, k);
    }
    int get_sum(int x) {
        int res = 0;
        while ( x > 0 ) {
            res += (x % 10);
            x /= 10;
        }
        return res;
    }
};
