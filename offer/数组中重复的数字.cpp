/*
在一个长度为n的数组里的所有数字都在0到n-1的范围内。 数组中某些数字是重复的，但不知道有几个数字是重复的。也不知道每个数字重复几次。请找出数组中任意一个重复的数字。 例如，如果输入长度为7的数组{2,3,1,0,2,5,3}，那么对应的输出是第一个重复的数字2。
*/

class Solution {
public:
    // Parameters:
    //        numbers:     an array of integers
    //        length:      the length of array numbers
    //        duplication: (Output) the duplicated number in the array number
    // Return value:       true if the input is valid, and there are some duplications in the array number
    //                     otherwise false
    // 所有的数都在 0 到 n-1 之间
    // 遍历数组，把每个数当作index，对数组对应index处的数+n，第二次遇到同一个数时，就会发现对应index处的数大于n
    bool duplicate(int numbers[], int length, int* duplication) {
        for ( int i = 0 ; i < length ; i++ ) {
            int index = (numbers[i] >= length) ? numbers[i] - length : numbers[i];
            if ( numbers[index] >= length ) {
                duplication[0] = index;
                return true;
            }
            numbers[index] += length;
        }
        return false;
    }
};
