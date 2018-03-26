"""
两个有序序列（长度不一定相等）中，求第K大的元素
前提 有序序列A，有序序列B
设 第 K 位的元素 = X ( k >= 1 ) 
假设 第K位的元素在 A 中，处于 A 的 第i位(A[i-1])，那么 A 中一定有 (i-1) 个元素 比 X 小 ，B中一定有（k-i) 个元素比 X小 (因为两个序列中一共有k-1个元素比X小) 

选取num1中的元素num1[idx1]（idx1 = n1/(n1+n2)*(k-1)），与num2中的元素num2[idx2]（idx2 = k-idx1-2, 即第k-idx1-1个元素）相比：
(这里有个不变的点：idx1及前面元素的个数 + idx2及前面元素的个数 = k，即（idx1+1）+（idx2+1）= k)
1. num1[idx1] == num2[idx2]，刚好有（idx1+1+idx2+1 = k）个元素不超过num1[idx1], 则vec1[idx1]为所求
2. num1[idx1] < num2[idx2]，将范围缩小到 num1[idx1+1:] 和 num2[0:idx2+2], 寻找 第（k-idx1-1)大的元素 (num1[idx1]及之前的元素一定小于X，num2[idx2+1]及之后的元素一定大于X) 
3. num1[idx1] > num2[idx2]，将范围缩小到 num2[idx2+1:] 和 num1[0:idx1+2], 寻找 第（k-idx2-1)大的元素

时间复杂度 : O(log(n1+n2))

求中位数就是这个算法的应用撒。。。
"""

num1 = [] 
num2 = [] 

def findKthLargest(n1,n2,k) : 
    """
    n1: 第一个有序序列
    n2: 第二个有序序列
    k: 要求第几位的元素 
    """
    if len(n1) == 0 :
        return n2[k-1] 
    if len(n2) == 0 :
        return n1[k-1]  
    if k == 1 : 
        return min(n1[0],n2[0])
    l1 = len(n1) ; l2 = len(n2) 
    index1 = (int)( l1 / (l1+l2) * ( k -1 ) )
    index2 = k - 2 - index1 
    if n1[index1] == n2[index2] :
        return n1[index1]
    elif n1[index1] < n2[index2] :                                      
        return findKthLargest(n1[index1+1:],n2[:index2+2],k-index1-1)           #  少了前面index1+1个元素，所以求 第 k-index1-1 大的元素 
    else :
        return findKthLargest(n1[:index1+2],n2[index2+1:],k-index2-1)           #  少了前面index2+1个元素，所以求 第 k-index2-1 大的元素 


if __name__ == '__main__' :
    num1 = [1,4,6,7,10,15,20] 
    num2 = [2,5,8,9,11]
    print(findKthLargest(num1,num2,4))
