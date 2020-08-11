# 找出第 k 小的距离对 

本题为：https://leetcode-cn.com/problems/find-k-th-smallest-pair-distance/ ， 代码在：https://github.com/yuyilei/LeetCode/blob/master/Array/Find-K-th-Smallest-Pair-Distance.cpp 。 

## 1. 统计所有数对差值的个数

一开始的想法就是统计出所有数对的差值，由于数组中的数字有重复，可以先计算所有不同数字的组成的数对之间的差值，然后乘以数字出现的次数，比如数字n1，n2分别出现c1，c2次，那么abs(n1-n2)出现的次数要增加c1*c2，并且对于出现过不止一次的数字，假设n出现了c次，那么0出现的次数要增加 c(c-1)/2 次。 

```C++
class Solution {
public:
    int smallestDistancePair(vector<int>& nums, int k) {
        map<int, long> sub;
        map<int, long> cnt;
        vector<int> un;
        int res;
        for (int n: nums) {
            if (cnt.find(n) == cnt.end()) {
                cnt[n] = 0;
                un.push_back(n);
            }
            cnt[n]++;
        }
        int len = cnt.size();
        for (int i = 0; i < len-1; i++) {
            for (int j = i+1; j < len; j++) {
                int tmp = abs(un[i]-un[j]);
                long all = cnt[un[i]] * cnt[un[j]];
                sub[tmp] += all;
            }
        }
        for (auto it: cnt) {
            sub[0] += (it.second)*(it.second-1)/2;
        }
        for (auto it: sub) {
            if (it.second < k) {
                k -= it.second;
            }
            else {
                res = it.first;
                break;
            }
        }
        return res;
    }
};

```

此算法的时间复杂度最差情况下为O(n^2)，所以会超时。 

## 2. 二分法+滑动窗口

尝试将算法的时间复杂度优化为O(nlogn)。

先对数组进行排序，就可以知道数组的最大和最小值了，可知第k大的距离一定在(0, 最大值-最小值)之间，将这两个数设为low和high，采用二分法，对于每个mid = (low+high)/2，统计数组中小于等于mid的距离的数组对个数，使用滑动窗口进行统计，设left，right为窗口的左右坐标，对于给定left的值越小，right的值越大，则距离 nums[right] - nums[left] 越大，对于每次统计出来距离小于等于mid的数组对个数count，如果count >= k，说明第k大的距离被包含在这count个数组对距离里面，所以令high = mid，反之，如果 count < k，说明第k大的距离，一定不在这count里面，所以令low = mid+1。 

```C++
class Solution {
public:
    int smallestDistancePair(vector<int>& nums, int k) {
        sort(nums.begin(), nums.end());
        int len = nums.size();
        int low = 0, high = nums[len-1] - nums[0];
        while (low < high) {
            int mid = (low+high)/2;
            int count = 0;
            int left = 0;
            for (int right = 0; right < len; right++) {                  // 逐渐增加right，左边缘left可以复用
                while (nums[right]-nums[left] > mid) left++;             // 对于给定的right，右移left，使nums[right]-nums[left]变小
                count += right - left;                                  
            }
            if (count >= k) {
                high = mid;
            }
            else {
                low = mid+1;
            }
        }
        return low;
    }
};
© 2020 GitHub, Inc.
```

外层循环是O(logn)，内层的滑动窗口由于每次的left是复用前一次的，所以虽然是双层循环，但内层时间复杂度为O(n)，总的时间复杂度为:O(nlogn)。
