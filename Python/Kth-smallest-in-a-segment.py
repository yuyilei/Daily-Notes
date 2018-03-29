"""
减治法：
在一个序列（无序）中查找第K小的元素。（K从0开始）
输入： 无序序列num，位置k(k从0开始)
输出： num中第k小的元素 
分析：
利用快速排序的划分的原理，以一个轴值为基准划分，一次划分之后，轴值左边的元素都比轴值小，右边的元素都比轴值大。
对num数组中下标left到right划分，划分之后，记轴值的为p
1) 如果 p = k, 轴值就是第K小的元素
2）如果 p > k, 第K小元素在num从 left 到 s-1 处
2）如果 p < k, 第K小元素在num从 s+1 到 right 处
继续查找，直到找到为止。
时间复杂度：最好 O(n), 最坏O(n^2), 平均O(n)
"""

num = [] 

def Partition(n,left,right) : 
    """
    n :  被查找的序列 
    left : 左下标
    right : 右下标
    """ 
    l = left ; r = right 
    while l < r : 
        while l < r and n[l] <= n[r] : 
            r -= 1 
        if  l < r :
            n[l], n[r] = n[r], n[l] 
            l += 1
        while l < r and n[l] <= n[r] : 
            l += 1 
        if l < r : 
            n[l], n[r] = n[r], n[l] 
            r -= 1 
    return l 

def searchKth(n,left,right,k) : 
    """
    n :  被查找的序列 
    left : 左下标
    right : 右下标
    k : 寻找第k位的数字 
    """
    p = Partition(n,left,right) 
    if p == k :
        return n[k]
    if p > k : 
        return searchKth(n,left,p-1,k) 
    if p < k :
        return searchKth(n,p+1,right,k) 


if __name__ == '__main__' :
    num = [5,3,8,1,4,6,9,2,7]
    k = 3 
    print(num,"数组中第",k+1,"小的元素是",searchKth(num,0,len(num)-1,k))