#include<queue>
using namespace std ; 

/*
使用优点队列，插入元素时，自动进行排序（默认是降序排序），内部使用堆排序实现（默认是大顶堆）
再说一点，要使用  memset函数，C中为<memory.h> 或 <string.h>。 C++中为<cstring> 虽然和这里没什么关系 
*/ 

struct node {
    int flag ; 
    bool operator< (const node tmp) const {                 //  重载 < 
        return flag > tmp.flag ;                             //  因为默认是大顶堆（降序），如果把 < 重载为 > 这样排序的时候就会按照小顶堆（升序)
    }
} ; 

int main() {
    priority_queue<int> descending_order1 ;                // 优先队列默认是降序 
    priority_queue<int,vector<int>,greater<int> > descending_order2 ;        // 等同是上面那个 
    priority_queue<int,vector<int>,less<int> > ascending_order ;             // 这个是升序 
    priority_queue<struct node> test ;                                // 会按照flag 升序排序 
    return 0 ; 
}