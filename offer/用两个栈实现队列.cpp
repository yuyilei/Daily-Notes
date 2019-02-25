/*
 * 用两个栈来实现一个队列，完成队列的Push和Pop操作。 队列中的元素为int类型。
 */

// stack1 存储队列元素，stack2做为缓冲区
// 入队列时，将元素 push 进stack1
// 出队列时，如果stack2不为空，返回stack2顶部的元素，否则，就将stack1中的元素依次倒入stack2，stack1中的最后一个元素就是队首元素
// 需要注意队列为空的情况

class Solution
{
public:
    void push(int node) {
        stack1.push(node);
    }

    int pop() {
        int res = 0;
        if ( !stack2.empty() ) {
            res = stack2.top();
            stack2.pop();
        }
        else if ( !stack1.empty() ) {
            while ( stack1.size() > 1 ) {
                stack2.push(stack1.top());
                stack1.pop();
            }
            res = stack1.top();
            stack1.pop();
        }
        return res;
    }

private:
    stack<int> stack1;
    stack<int> stack2;
};
