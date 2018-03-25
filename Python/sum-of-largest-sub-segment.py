# 求最大子段和，并找出左右两边的index。。
# 感觉自己python写的太烂了。。。之后要试试用python写算法。。。
# (๑◔‿◔๑) python  已经不会写了 sad !!!   

"""
分治法（递归）:
对于  list[left,right] 
最大最大子段和有三种情况:
1. MaxSub(left,right) == MaxSub(left,middle) 
2. MaxSub(left,right) == MaxSub(middle+1,right) 
3. MaxSub(left,right) = sum(list[i,j]) 其中，1 <= i <= (left+right) / 2 , (left+right)/2 + 1 <= j <= right 
所以分着分着。。就分到了 left == right 的情况。。。
时间复杂度 ： O(n*log(n))
空间复杂度 ： O(1) 
"""

nums = []

def SumSubSeg1(left,right) : 
    """
    left: 所求最大子段和数组的左下标
    right: 所求最大子段和数组的右下标
    """ 
    if  left == right : 
        return max(nums[left],0)            # 如果它是负数的话，那就不取它。。。子段，可以是空

    middle = (left + right) // 2            # 地板除。。。。  
    lsum = SumSubSeg1(left,middle) 
    rsum = SumSubSeg1(middle+1,right) 

    sum1 = 0 ; lmax = 0 
    for i in range(middle,left-1,-1) :        # 顺序为 middle...left ，然后用range反一下。。
        sum1 += nums[i] 
        lmax = max(sum1,lmax)

    sum2 = 0 ; rmax = 0 
    for i in range(middle+1,right+1) :        # 顺序为 middle+1...right， 就是正序，不用反 
        sum2 += nums[i] 
        rmax = max(sum2,rmax)

    lrsum = lmax + rmax
    return max(lrsum,lsum,rsum) 

"""
动态规划：
已知前n个数的最大子段和为  temp 
那么前n+1个个数的最大子段和 为 max(temp+nums[n+1],nums[t+1]) 
tmp[i] 为 0  到 i 的数组的最大子段和 
时间复杂度 : O(n)
空间复杂度 : O(1)
"""

def SumSubSeg2() : 
    size = len(nums) 
    tmp = 0 ; maxx = 0  
    begin = 0 ; end = 0 
    """
    tmp : 在判断 前 n+1 个数的子段和时，前 n 个数的最大子段和（一定包括nums[i]）
    maxx : 记录目前算得的最大子段和 ( 不一定包括nums[i])   
    begin : 最大子段和开始的位置
    end: 最大子段和结束的位置 
    """
    for i in range(0,size) : 
        if tmp > 0 :                  #  tmp > 0，说明前 n+1 个数的最大子段和要包括 前 n 个数的最大子段和 
            tmp += nums[i]  
        else :                        #  tmp <= 0, 说明 前 n + 1 个数的最大子段和 是自己本身 ， 记下开始下标 
            tmp = nums[i] 
            begin = i                
        if maxx < tmp :               #  刷新了最大子段和的记录， 保存 结束位置的下标 
            maxx = tmp 
            end = i 

    if begin > end :                 #  出现这种情况就说明，最大子段和序列为空。。。就把index都设为-1 
        begin = end = -1  
    return begin, end, maxx  

if __name__ == '__main__' : 
    nums = [-20,11,-4,13,-5,-1]               # list 全局变量改变的时候，不用global。。。全忘了啊啊啊啊啊！ 
    #nums = [-1,-4,-5,-4]
    print(SumSubSeg1(0,len(nums)-1))
    print(SumSubSeg2())
    
    
    