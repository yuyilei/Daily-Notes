#include<iostream>
#include<cstdio>
#include<cstring>
using namespace std ; 

class KMP {
    private : 
    char *T ;                   // 子串
    char *S ;                   // 主串 
    int  slen, tlen ;           // 主串长度，子串长度
    int *next ;                 // 子串的next的数组 
    bool ifnext ; 
    public : 

    KMP() {
        cout << "输入主串长度，子串长度:" << endl ; 
        cin >> slen >> tlen ; 
        T = new char [tlen+2] ; 
        S = new char [slen+2] ;
        next = new int [tlen+2] ; 
        cout << "输入主串:" << endl ; 
        scanf("%s",S) ; 
        cout << "输入子串:" << endl ; 
        scanf("%s",T) ; 
        ifnext = false ; 
    }

    void getNext() {
        int i, j, maxlen ; 
        ifnext = true ; 
        next[0] = -1 ; 
        for ( j = 1 ; T[j] != '\0' ; j++ ) {                                     // 依次求 next[j]
            for ( maxlen = j - 1 ; maxlen >= 1 ; maxlen-- ) {                    // 从next[j] 可能的最大值maxlen开始 ,求得maxlen的值
                for ( i = 0 ; i < maxlen ; i++ ) 
                    if ( T[i] != T[j-maxlen+i] ) 
                        break ; 
                if ( i == maxlen ) {                                              // next[j] 的值                                      
                    next[j] = maxlen ; 
                    break ; 
                }
            }
            if ( maxlen < 1 )                                                     
                next[j] = 0 ; 
        }
    }

    int kmp() {
        if ( ifnext == false ) 
            getNext() ; 
        int i = 0, j = 0 ;
        while ( S[i] != '\0' && T[j] != '\0' ) {
            if ( S[i] == T[j] ) {
                i++ ; 
                j++ ; 
            } 
            else {
                j = next[j] ;                                                    // 匹配不成功 子串从 next[j] 开始 
                if ( j == -1 ) {                                                  
                    i++ ; 
                    j++ ; 
                }
            }
        }
        if ( T[j] == '\0' ) {
            return ( i - tlen ) ; 
        }
        return -1 ; 
    }
    ~KMP() {
        delete [] T ; 
        delete [] S ; 
    }
} ; 


void test() {
    KMP example ; 
    int index = example.kmp() ; 
    if ( index == -1 ) 
        cout << "子串在主串中不匹配!" << endl ; 
    else 
        cout << "子串在主串下标 " << index << " 处匹配!" << endl ; 
}

int main() {
    test() ; 
    return 0 ; 
}