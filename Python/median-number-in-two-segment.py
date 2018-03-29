"""
两个有序序列，查找其中位数。 
前提：两个长度相同的有序序列，A，B
1. 分别求两个序列的中位数,a,b
2. 
   1. a == b ，则 a 就是两个序列的中位数 
   2. a > b , 中位数只能出现在 a , b 中间， 舍弃 a 后面的部分序列得到A1， 舍弃 b 前面的部分序列得到B1 
   3. a < b , 中位数只能出现在 a , b 中间， 舍弃 a 前面的部分序列得到A1， 舍弃 b 后面的部分序列得到B1
3. 重复步骤1和2，直到两个序列中分别只有一个数字，两者中的较小值就是中位数。 (注意过程中要保证A和B的长度相等)

这个算法的关键就是，要保持两个序列的长度相等（就是说每次舍弃掉的序列的长度也要相等） 
因为两个序列的长度相等，所以两个序列的元素总和是偶数个，这里默认取较小的那个个中位数，如果有别的需要，修改一下返回就行 
时间复杂度 : O(logn)
空间复杂度 : O(1)
"""

num1 = [] ; num2 = []
def findMedian() :  
    s1 = 0 ; s2 = 0 
    e1 = len(num1) - 1 ; e2 = len(num2) - 1 
    while s1 < e1 and s2 < e2 : 
        m1 = ( s1 + e1 ) // 2 
        m2 = ( s2 + e2 ) // 2 
        if num1[m1] == num2[m2] : 
            return num1[m1] 
        elif num1[m1] > num2[m2] :               # 舍弃掉 num1的后一部分和 num2的 前一部分 
            s2 = s2 + e1 - m1                    # 舍弃的长度为 e1 - m1 
            e1 = m1 
        else :                                   # 舍弃掉 num2的后一部分和 num2的 后一部分
            s1 = s1 + e2 - m2                    # 舍弃的长度为 e2 - m2  
            e2 = m2 
    return min(num1[s1],num2[s2])                # 返回最小值是因为这里要返回 较小的那个中位数 
    # return (num1[s1]+num2[s2]) / 2             # 这个值是两个中位数的平均值 
    # return max(num1[s1],num2[s2])              # 也可以返回两个中位数的较大者 


if __name__ == '__main__' : 
    #num1 = [11,13,16,17,19,23] 
    #num2 = [2,4,10,14,20,21]
    num1 = [1,2,4,5,6,7] 
    num2 = [11,12,13,14,15,16]
    print(findMedian()) 