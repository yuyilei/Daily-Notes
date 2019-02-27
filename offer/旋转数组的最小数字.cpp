/*
把一个数组最开始的若干个元素搬到数组的末尾，我们称之为数组的旋转。 输入一个非减排序的数组的一个旋转，输出旋转数组的最小元素。 例如数组{3,4,5,1,2}为{1,2,3,4,5}的一个旋转，该数组的最小值为1。 NOTE：给出的所有元素都大于0，若数组大小为0，请返回0。
 */

// 1 2 3 4 5 6
// 4 5 6 1 2 3
// 1 2 3 4 5
// 4 5 1 2 3
class Solution {
public:
    int minNumberInRotateArray(vector<int> array) {
        if ( array.empty() )
            return 0;
        int left = 0, right = array.size()-1;
        while ( left <= right ) {
            int mid = (left + right)/2;
            if ( array[mid] > array[right] ) // 此时最小值在右边，且不可能为array[mid]
                left = mid + 1;
            else if ( array[left] > array[mid] ) // 此时最小值在左边，有可能为array[mid]
                right = mid;
            else                               // 此时这个数组里面的数全部相等
                right--;
        }
        return array[left];
    }
};
