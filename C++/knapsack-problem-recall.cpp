#include<iostream>
using namespace std ; 

// 用栈解决背包问题 。。 自定义的栈23333 
typedef int item ; 
int num = 6 ; 
item values[6] = {2,5,8,3,4,6} ; 
int target = 12 ; 
const int STACK_INIT_NUM = 100 ; 
const int STACKUP = 10 ; 
typedef struct {
    item *elem ;  
    int top, size, upsize ; 
} Sqstack ; 

void init_stack(Sqstack & s, int size = STACK_INIT_NUM, int upsize = STACKUP) {
    s.elem = new item(size) ; 
    s.top = -1 ; 
    s.size = size ; 
    s.upsize = upsize ; 
} 

bool get_top(Sqstack & s, item &e ) {
    if ( s.top == -1 ) 
        return false ; 
    e = s.elem[s.top] ; 
    return true ; 
}

void push_top(Sqstack & s, item & e) {
    s.elem[++s.top] = e ; 
}

bool pop_top(Sqstack & s, item &e ) {
    if ( s.top == -1 ) 
        return false ; 
    e = s.elem[s.top--] ; 
    return true ; 
}

bool traverse(Sqstack & s) {
    if ( s.top == -1 ) 
        return false ; 
    for ( int i = 0 ; i <= s.top ; i++ ) 
        cout << values[s.elem[i]] << " " ; 
    cout << endl ; 
    return true ; 
}

bool is_Empty(Sqstack & s) {
    if ( s.top == -1 ) 
        return true ; 
    return false ; 
}

void knapsack(int t, int n) {
    int index = 0 ; 
    Sqstack s ; 
    init_stack(s) ; 
    do {
        while ( t > 0 && index < n ) {
            if ( t - values[index] >= 0 ) {           // 如果第index件物品加入之后没有超出，就加入这件物品 
                push_top(s,index) ; 
                t -= values[index] ; 
            }
            index++ ; 
        }
        if ( t == 0 )                    // 如果背包内的物品达到需要的值 
            traverse(s) ;                // 输出一组解，之后回溯 
        pop_top(s,index) ;               // 退出栈顶物品，背包容量增加 
        t += values[index] ;             // 继续考察下一件物品. 
        index++ ; 
    } while ( !is_Empty(s) || index < n ) ; // 若栈不为空或者仍有可选物品，就可以继续回溯  
}

int main() {
    knapsack(target,num) ; 
    return 0 ; 
}