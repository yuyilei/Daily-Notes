#include<cstdio>
#include<iostream> 
#include<algorithm> 
#include<vector> 
using namespace std;

/*
有2元，3元，5元的硬币若干枚，用数量最少的硬币来构成 n 元 
思路动态规划 
*/
struct num {
    int r2, r3, r5, sum; 
    bool flag; 
}; 

int main() {
    int n, c2, c3, c5;
    cin >> c2 >> c3 >> c5 >> n;
    if ( n == 1 || 2*c2+3*c3+5*c5 < n ) {
        cout << "can not!" << endl;
        return 0;
    }
    vector<struct num> dp (n+1);
    if ( c2 >= 1 ) {
        dp[2].r2 = dp[2].sum = 1;
        dp[2].flag = true; 
    }
    if ( c3 >= 1 ) {
        dp[3].r3 = dp[3].sum = 1;
        dp[3].flag = true; 
    }
    if ( c2 >= 2 ) {
        dp[4].r2 = dp[4].sum = 2;
        dp[4].flag = true;
    }
    if ( c5 >= 1 ) {
        dp[5].r5 = dp[5].sum = 1;
        dp[5].flag = true; 
    }
    for ( int i = 6 ; i <= n ; i++ ) {
        int tmp = i;
        int step = 0; 
        if ( dp[i-2].flag && dp[i-2].r2 < c2 && tmp > dp[i-2].sum ) {
            tmp = dp[i-2].sum;
            step = 2;
        }
        if ( dp[i-3].flag && dp[i-3].r3 < c3 && tmp > dp[i-3].sum ) {
            tmp = dp[i-3].sum;
            step = 3;
        }
        if ( dp[i-5].flag && dp[i-5].r5 < c5 && tmp > dp[i-5].sum ) {
            tmp = dp[i-5].sum;
            step = 5;
        }
        
        if ( step != 0 ) {
            dp[i].r2 = dp[i-step].r2; 
            dp[i].r3 = dp[i-step].r3; 
            dp[i].r5 = dp[i-step].r5; 
            dp[i].flag = true;
            if ( step == 2 ) 
                dp[i].r2++;
            else if ( step == 3 ) 
                dp[i].r3++;
            else 
                dp[i].r5++;
            dp[i].sum = dp[i].r2 + dp[i].r3 + dp[i].r5; 
        }
    }
    
    if ( dp[n].flag == false ) {
        cout << "can not!" << endl;
    }
    else {
        cout << dp[n].r2 << " " << dp[n].r3 << " " << dp[n].r5 << endl;
    }
    return 0; 
}