/*
如何得到一个数据流中的中位数？如果从数据流中读出奇数个数值，那么中位数就是所有数值排序之后位于中间的数值。如果从数据流中读出偶数个数值，那么中位数就是所有数值排序之后中间两个数的平均值。我们使用Insert()方法读取数据流，使用GetMedian()方法获取当前读取数据的中位数。
*/

/*
 牛客网上看到的解答：
 维持一个 大顶堆，和一个小定堆
 保持大顶堆中所有的元素都小于等于大顶堆，且大顶堆的元素个数等于小顶推或者等于小顶堆减一
 元素的中位数为大顶堆和小顶堆的堆顶元素之平均或者小顶堆的堆顶元素 
 */
class Solution {
public:
    void Insert(int num) {
        if ( minq.empty() || num < minq.top() )
            minq.push(num);
        else
            maxq.push(num);
        // push 了元素之后可能会出现两种情况需要调整
        // 1. 小顶堆中的元素比大顶堆多2个
        if ( minq.size() == maxq.size() + 2 ) {
            maxq.push(minq.top());
            minq.pop();
        }
        // 2. 大顶堆中的元素比小顶堆多1个
        if ( minq.size() + 1 == maxq.size() ) {
            minq.push(maxq.top());
            maxq.pop();
        }
    }
    double GetMedian() {
        if ( minq.size() == maxq.size() )
            return (minq.top()+maxq.top())/2.0;
        return minq.top()/1.0;
    }
private:
    priority_queue<int, vector<int>, less<int> > minq;
    priority_queue<int, vector<int>, greater<int> > maxq;
};
