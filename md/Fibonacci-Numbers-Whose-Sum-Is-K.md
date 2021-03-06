# 和为 K 的最少斐波那契数字数目 

本题是leetcode上的一道题，求和为k的最少斐波那契数字的数目，题目链接： https://leetcode-cn.com/problems/find-the-minimum-number-of-fibonacci-numbers-whose-sum-is-k/ ， 代码在：https://github.com/yuyilei/LeetCode/blob/master/Array/Find-the-Minimum-Number-of-Fibonacci-Numbers-Whose-Sum-Is-K.cpp 。 

本题可以用贪心法，先找出小于等于k的斐波那契数字，再从大到小取数字，如果当前斐波那契数小于等于当前的k，就取出当前斐波那契数，并将k减去这个斐波那契数，数目加一。 

证明为何每次要取最大的斐波那契数，即对于任意一个k，它的最优方案一定有小于等于k的最大斐波那契数。 

下面证明：

1. 最佳方案不会选择两个连续的斐波那契数。 

    由斐波那契数性质可知， Fn = Fn-1 + Fn-2，因此，如果要选取Fn-1和Fn-2可以用Fn代替。 

2. 最佳方案可以不选择两个相同的斐波那契数。 

    由斐波那契数的性质可知， 2*Fn = Fn + Fn = Fn + Fn-1 + Fn-2 = Fn+1 + Fn-2，因此两个Fn可以用Fn+1和Fn-2代替。

3. 对于任意给定的k，最佳方案需要选择小于等于k的最大斐波那契数。 

    假设对于给定k，最佳方案不选择小于等于k的最大斐波那契数Fm，有1和2证明：不会选择两个连续的斐波那契数，也可以不选择两个相同的斐波那契数，所以用F1,..,Fm-1的数可以构造出的最大和为： 

    当m为奇数时，max_sum = Fm-1 + Fm-3 + ... + F4 + F2 = Fm-1 + Fm-3 + ... + F4 + F2 + F1 - F1 = Fm - F1 = Fm - 1 (加上F1，再减去F1)
    
    当m为偶数时，max_sum = Fm-1 + Fm-3 + ... + F3 + F1 = Fm-1 + Fm-3 + ... + F3 + F2 = Fm (因为F1 = F2) 

    可知，m为奇数时，能构成的最大的数为Fm-1，小于Fm，也就小于k，无法构成k，m为偶数时，虽然能构成Fm，但是个数比只选一个Fm多。

综上，对于任意一个k，它的最优方案一定有小于等于k的最大斐波那契数。  


