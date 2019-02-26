/*
给定一个数组和滑动窗口的大小，找出所有滑动窗口里数值的最大值。例如，如果输入数组{2,3,4,2,6,2,5,1}及滑动窗口的大小3，那么一共存在6个滑动窗口，他们的最大值分别为{4,4,6,6,6,5}； 针对数组{2,3,4,2,6,2,5,1}的滑动窗口有以下6个： {[2,3,4],2,6,2,5,1}， {2,[3,4,2],6,2,5,1}， {2,3,[4,2,6],2,5,1}， {2,3,4,[2,6,2],5,1}， {2,3,4,2,[6,2,5],1}， {2,3,4,2,6,[2,5,1]}。
*/

class Solution {
public:
    vector<int> maxInWindows(const vector<int>& num, unsigned int size) {
        vector<int> res;
        if ( num.empty() || num.size() < size || size == 0 )
            return res;
        // 维持一个单调队列，队首元素是当前窗口中最大元素的index
        deque<int> q;
        for ( unsigned int i = 0 ; i < num.size() ; i++ ) {
            // 从后面依次弹出队列中比当前num值小的元素，因为这些元素不会是窗口的最大值
            while ( q.size() && num[q.back()] <= num[i] )
                q.pop_back();
            // 从前面依次弹出队列中不在当前窗口的元素，因为这些元素过期了
            while ( q.size() && i-q.front()+1 > size )
                q.pop_front();
            // 在队尾放入当前index
            q.push_back(i);
            // 只有“形成”窗口之后，再在res中放入值
            if ( i >= size - 1 )
                res.push_back(num[q.front()]);
        }
        return res;
    }
};

