#include<iostream>
using namespace std ;
int keys[20] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20} ; 
int num = 20 ; 
void showelem() {
    for ( int i = 0 ; i < num ; i++ ) 
        cout << keys[i] << " " ;
    cout << endl ; 
}

void invert(int s, int e) {
    for ( int i = s ; 2*i < (s+e) ; i++ ) {
        keys[i] = keys[i] + keys[e-i+s] ; 
        keys[e-i+s] = keys[i] - keys[e-i+s] ;
        keys[i] = keys[i] - keys[e-i+s] ; 
    }
}

int main(){
    int n, m, i ;
    cout << "输入n,m ———— 将数组的前n个和后m个元素整体交换（0 < n < 20 , 0 < m < 20 , 0 < n+m <= 20)" << endl ; 
    cin >> n >> m ; 
    cout << "反转前:" << endl ; 
    showelem() ; 
    invert(0,num-1) ;
    invert(0,m-1) ;
    invert(m,num-n-1) ; 
    invert(num-n,num-1) ; 
    cout << "反转后:" << endl ; 
    showelem() ; 
    return 0 ; 
}