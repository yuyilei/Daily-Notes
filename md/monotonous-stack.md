# 单调栈

单调栈就是元素在入栈的时候，遵守单调原则，因此可以求出一个元素向左和向右所能扩展的最大长度(>=或者<=这个元素)。

例如：求一个元素的右侧的最近比他大的元素位置。

单调栈通过维护栈内元素单调递增或者单调递减，判断 **栈顶元素** 和 **当前元素** 的关系，然后得到一个元素向左和向右所能扩展的最大长度。

## 柱状图中的最大矩形

给定 n 个非负整数，用来表示柱状图中各个柱子的高度。每个柱子彼此相邻，且宽度为1。求在该柱状图中，能够勾勒出来的矩形的最大面积。例如：其中每个柱子的宽度为1，给定的高度为 [2,1,5,6,2,3]。

本题需要维持一个递增栈，遇到大于栈顶元素的当前元素，就把当前如栈，否则出栈。 

```c++
    int largestRectangleArea(vector<int>& heights) {
        stack<int> index;
        heights.push_back(0);                        // push一个0，可以处理完最后一个元素。
        int i = 0, res = 0;
        while ( i < heights.size() ){
            if ( index.empty() || heights[index.top()] <= heights[i] ){
                index.push(i++); 
            } 
            else{
                int t = index.top(); 
                index.pop(); 
                res = max(res,heights[t]*(index.empty()?i:i-index.top()-1));
            }
        }
        return res;
```


## 倒水

地上从左到右竖立着 n 块木板，从 1 到 n 依次编号。已知每块木板的高度，在第 n 块木板右侧竖立着一块高度无限大的木板，现对每块木板依次做如下的操作：

对于第 i 块木板，从其右侧开始倒水，直到水的高度等于第 i 块木板的高度，倒入的水会淹没 ai 块木板（如果木板左右两侧水的高度大于等于木板高度即视为木板被淹没），

求 n 次操作后，最多能倒入水的体积是多少（最大的ai）。

例如，在第 4 块木板右侧倒水，可以淹没第 5 块和第 6 块一共 2 块木板，a4 = 2。

如图：

![图片](https://cnblogsimages2017.oss-cn-hangzhou.aliyuncs.com/blog/1055307/201712/1055307-20171214173401904-448316124.png)

本题需要维持一个单调递减栈。

```c++
    int largestWaterArea(vector<int>& heights) {
        stack<int> index; 
        heights.push_back(99999);                           // 加入最右端的无限高的木板，可以处理最后一个元素。
        int i = 0, res = 0;
        while ( i < heights.size() ){
            if ( index.empty() || heights[top.size()] >= heights[i] ){
                index.push(i++); 
            }
            else {
                int t = index.top();
                index.pop();
                res = max(res,heights[t]*(i-t));
            }
        }
        return res; 
    } 
```
