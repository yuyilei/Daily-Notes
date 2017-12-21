#include<cstdio>
#include<iostream> 
using namespace std ; 

void swap(int &a, int &b) {
    a = a + b ;  
    b = a - b ; 
    a = a - b ;
}


// 传统的辗转相除法： 
// 缺点：当a和b都很大时，a%b效率低   
// 优点：当a，相差很大时，效率高 
int gcd1(int a, int b) {         
    if ( a%b == 0 ) 
        return b ; 
    return gcd1(b ,a % b) ; 
} 

// 更相减损术： 
// 缺点：a,b相差很大的时候，效率很低 
// 优点：当a,b都很大时，a-b效率高 
int gcd2(int a, int b ) {
    if ( a == b ) 
        return a ; 
    if ( a < b ) 
        swap(a,b) ; 
    return gcd2(b,a-b) ; 
}

// 更相减损术与移位结合： 
// 避免了取模运算，而且算法性能稳定。 
int gcd3(int a, int b ) {
    if ( a == b ) 
        return a ; 
    if ( a < b ) 
        swap(a,b) ; 
    if ( (a%2) && (b%2) )    // a,b都是奇数时，使用更相减损发，此时： a-b一定是偶数 
        return gcd3(b,a-b) ; 
    if ( !(a%2) && (b%2) )   // a是偶数，b是奇数， a,b的最大公约数，就是a/2和b之间的公约数 
        return gcd3(a/2,b) ; 
    if ( (a%2) && !(b%2) ) 
        return gcd3(a,b/2) ;  // a是奇数，b是偶数，同理 
    if ( !(a%2) && !(b%2) ) 
        return 2*gcd3(a/2,b/2) ;   // a,b都是偶数，gcd3(a,b) == 2*gcd3(a/2,b/2)  
}


int main() {
    int a, b ; 
    cin >> a >> b ; 
    if ( a < b ) 
        swap(a,b) ; 
    cout << "int gcd1 : " << gcd1(a,b) << endl ;     
    cout << "int gcd2 : " << gcd2(a,b) << endl ;     
    cout << "int gcd3 : " << gcd3(a,b) << endl ;     
}

