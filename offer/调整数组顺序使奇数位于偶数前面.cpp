/*
输入一个整数数组，实现一个函数来调整该数组中数字的顺序，使得所有的奇数位于数组的前半部分，所有的偶数位于数组的后半部分，并保证奇数和奇数，偶数和偶数之间的相对位置不变。
 */

/*
时间复杂度: O(n)
空间复杂度: O(n)
*/
class Solution {
public:
    void reOrderArray(vector<int> &array) {
        vector<int> res;
        for ( int num : array ) {
            if ( num % 2 == 1 )
                res.push_back(num);
        }
        for ( int num : array ) {
            if ( num % 2 == 0 )
                res.push_back(num);
        }
        array = res;
    }
};

/*
时间复杂度: O(n)
空间复杂度: O(n) 
 */
class Solution {
public:
    void reOrderArray(vector<int> &array) {
        for ( int i = 0 ; i < array.size() ; i++ ) {
            for ( int j = array.size()-1 ; j > 0 ; j-- ) {
                if ( array[j]%2 == 1 && array[j-1]%2 == 0 )
                    swap(array[j],array[j-1]);
            }
        }
    }
};
