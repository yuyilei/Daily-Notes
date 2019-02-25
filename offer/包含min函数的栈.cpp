/*
定义栈的数据结构，请在该类型中实现一个能够得到栈中所含最小元素的min函数（时间复杂度应为O（1））。
 */

class Solution {
public:
    void push(int value) {
        s.push(value);
        if ( ms.empty() || ms.top() >= value ) {
            ms.push(value);
        }
    }
    void pop() {
        if ( s.empty() )
            return;
        int res = s.top();
        s.pop();
        if ( !ms.empty() && ms.top() == res ) {
            ms.pop();
        }
    }
    int top() {
        if ( !s.empty() )
            return s.top();
        return 0;
    }
    int min() {
        if ( !ms.empty() )
            return ms.top();
        return 0;
    }
 private:
    stack<int> ms, s;
};
