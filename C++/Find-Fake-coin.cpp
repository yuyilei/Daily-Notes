#include<cstdio>
using namespace std ; 

/*
减治法
有n个硬币，其中有一个假币，比真币轻，有一台天平，能比较任意两堆硬币的质量。 比较多少次能找出假币。
输入：
一个硬币的数组，硬币的左右下标为left和right，硬币的个数。
输出：
假币在硬币数组位置。 
思路：
将硬币分为3堆，其中两堆为[n/3]记为a1,a2，剩下的是a3
比较a1和a2的质量：
如果 a1 > a2，假币在a2中，就在a2中查找。
如果 a1 < a3，假币在a1中，就在a1中查找。
如果 a1 == a2，假币在a3中，就在a3中查找。 
*/

int weight[11] = {2,2,2,2,1,2,2,2,2,2,2} ; 
int FindFakeIndex(int left, int right ) {
    int n = right - left + 1 ;                         //   硬币的数量 
    int num1, num2, num3 ;                             //   3堆硬币的数量
    if ( n == 1 ) 
        return left ; 
    if ( n % 3 == 0 )                                 // 保证硬币的数量是 [n/3]
        num1 = num2 = n / 3 ;
    else 
        num1 = num2 = n / 3 + 1 ; 
    num3 = n - num1 - num2 ; 
    int sum1 = 0, sum2 = 0 ;    
    for ( int i = 0 ; i < num1 ; i++ ) 
        sum1 += weight[i+left] ; 
    for ( int i = num1 ; i < num1+ num2 ; i++ ) 
        sum2 += weight[i+left] ; 
    if ( sum1 < sum2 ) 
        return FindFakeIndex(left,left+num1-1) ; 
    else if ( sum1 > sum2 ) 
        return FindFakeIndex(left+num1,left+num1+num2-1) ; 
    else 
        return FindFakeIndex(left+num1+num2,right) ; 
}

int main() {
    int index = FindFakeIndex(0,10) ; 
    printf("假币的位置为: %d\n",index) ; 
    return 0 ; 
}