# 整体交换数组的前n个元素和后m个元素 

如果想要交换 一个数组的前n个元素和后m个元素，比如：

一个数组原先是:

**1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20**

整体交换前3个元素和后7个元素之后是：

**14 15 16 17 18 19 20 4 5 6 7 8 9 10 11 12 13 1 2 3**

常规的方法操作比较繁琐，时间复杂度比较高，为 `O(m*n)`，而且移动时，需要一定量的辅助空间。

另一种解法的时间复杂度为 `O(num)` （num为数组长度)，且不需要辅助空间  --> [demo](https://github.com/yuyilei/Algorithms/blob/master/C%2B%2B/change-n-m.cpp)

### 交换两个元素

```
    void invert(int s, int e) {
        for ( int i = s ; 2*i < (s+e) ; i++ ) {
            keys[i] = keys[i] + keys[e-i+s] ; 
            keys[e-i+s] = keys[i] - keys[e-i+s] ;
            keys[i] = keys[i] - keys[e-i+s] ; 
        }
    }
```

有一种交换操作不需要辅助元素： 

case1: 

```
    int a, b ; 
    { 
        a = a + b ;
        b = a - b ; 
        a = a - b ; 
    }
```

需要注意的是： 这种交换不适合用于**指针指向的元素**的交换 ：

case2: 

```
    int *a, *b ; 
    {
        *a = *a + *b ; 
        *b = *a - *b ; 
        *a = *a - *b ; 
    }
```

在**case1**中，如果 a = b = 3， 交换的结果会是 a = b = 3 , 没有问题。 

在**case2**中，如果 a = b = &i, i = 3, 交换的结果就会出错，变成 \*a = \*b = 0 。

原因是，这里的a和b是指针，指向同一个地址，改变\*a就会改变\*b，所以最后\*a和\*b都会变成0，原本的\*a和\*b丢失了。  

所以，这种交换不适合**指针指向的元素**的元素，如果一定要用，就要确保交换的这两个元素不相等（两个指针是否指向一个地址)。

数组的本质是指针，所以在移动数组元素时，如果使用这种方法，就要保证不能交换同一个元素。 

还有一种交换，也不需要辅助空间 : 

```
    int a, b ; 
    {
        a = a ^ b ; 
        b = a ^ b ;
        a = a ^ b ; 
    }
```
但是，对于**指针指向的元素**：

```
    int *a, *b ; 
    {
        *a = *a ^ *b ; 
        *b = *a ^ *b ;
        *a = *a ^ *b ; 
    }
```
同理，不要使用这种方法交换**同一个指针指向的元素**！！

### 四步翻转

**invert**函数将数组下标从s到e的元素翻转。 

只需进行四步翻转： 

```
    {
        invert(0,num-1) ;
        invert(0,m-1) ;
        invert(m,num-n-1) ; 
        invert(num-n,num-1) ; 
    }
```
以 **1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20**  (n = 3, m = 7) 为例： 

首先，将元素整体反转，得到： 

**20 19 18 17 16 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1** 

再将前m个元素翻转，得到：

**14 15 16 17 18 19 20 13 12 11 10 9 8 7 6 5 4 3 2 1**

再将中间的元素翻转，得到：

**14 15 16 17 18 19 20 4 5 6 7 8 9 10 11 12 13 3 2 1**

最后，将后n个元素翻转，得到：

**14 15 16 17 18 19 20 4 5 6 7 8 9 10 11 12 13 1 2 3**

这就是所求结果。

总体上，只遍历了整个数组2遍，所以时间复杂度为`O(num)`（num为数组长度) ，且不需辅助空间（除去遍历需要的下标**i**)。 
## 另解
填坑法
1. 记`m,n`最大公约数为`k`, 迭代步骤2-4`k`次
2. 对第`i`次(从`0`开始)取出`pos = key[i]`, 记录当前索引为`i`, 当前索引所对应源数组索引关系见`indexof`函数, 更换索引值为函数结果.
3. 迭代步骤2
4. 索引再次为零时, 放置`pos`.

代码如下: 复杂度`o(n)`,无额外开销

```c
#include <stdio.h>
int indexof(int i, int m, int n, int t){
	if(i<n) return t-n+i;
	else if(i<t-m) return i-n+m;
	else return i+m-t;
}
int nau(int m,int n){
	int max = (m>n)?m:n;
	int min = (m+n)-max;
	int res;
	while((res=max-min)){
		max = (min > res)?min:res;
		min = (min + res) - max;
	}
	return max;
}

int main(int argc, char const* argv[]){
	int key[20] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};
	for(int N = 0; N < 20; N ++) printf("%3d",key[N]); 
	printf("\n");
	int m,n;
	scanf("%d%d",&m,&n);
	int mx = nau(m,n);
	for(int i=0;i < mx;i++){
		int space = i;
	  int pos = key[space];
	  do{
		  int temp = indexof(space,m,n,20);
	  	key[space] = (temp==i)?pos:key[temp];
  		space = temp;
  	}while(space != i);
	}
	for(int N = 0; N < 20; N ++) printf("%3d",key[N]); 
	return 0;
}
```
