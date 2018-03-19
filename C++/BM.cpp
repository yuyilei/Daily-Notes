#include<iostream>
#include<cstdio>
#include<cstring>
using namespace std ;

class BM {
    private : 
        char *T ; 
        char *S ; 
        int tlen ; 
        int slen ; 
        int *BmGs ; 
        int *BmGc ; 
        int *suff ; 

    public :  
        BM() {
                cout << "输入主串长度，子串长度:" << endl ; 
                cin >> slen >> tlen ; 
                T = new char [tlen+2] ; 
                S = new char [slen+2] ;
                BmGc = new int [tlen+2] ; 
                BmGs = new int [tlen+2] ; 
                suff = new int [tlen+2] ; 
                cout << "输入主串:" << endl ; 
                scanf("%s",S) ; 
                cout << "输入子串:" << endl ; 
                scanf("%s",T) ; 
        }
        
        void getBmGc() {
            for ( int i = 0 ; i < 256 ; i++ ) 
                BmGc[i] = tlen ;  
            for ( int i = 0 ; i < tlen ; i++ ) 
                BmGc[T[i]] = tlen - 1 - i ; 
        }

        void getsuff() {
            suff[tlen-1] = tlen ; 
            for ( int i = tlen - 2 ; i >= 0 ; i-- ) {
                int tmp = i ; 
                while ( tmp >= 0 && T[tmp] == T[tlen-1-i+tmp] ) 
                    tmp-- ; 
                suff[i] = i - tmp ; 
            }
        }

        void getBmGs() {
            getsuff() ; 
            for ( int i = 0 ; i < tlen ; i++ ) 
                BmGs[i] = tlen ; 

            int j = 0 ; 
            for ( int i = tlen - 1 ; i >= 0 ; i-- ) {
                if ( suff[i] == i+1 ) 
                    for ( ; j < tlen - 1 - i ; j++ ) 
                        if ( BmGs[j] == tlen ) 
                            BmGs[j] = tlen - i - 1 ; 
            }

            for ( int i = 0 ; i <= tlen - 2 ; i++ ) 
                BmGs[tlen-1-suff[i]] = tlen - i - 1 ; 
        } 


        void bm() {
            getBmGc() ; 
            getBmGs() ;     
            int i, j, flag = 0 ; 
            for ( j = 0 ; j <= slen - tlen ; ) {
                for ( i = tlen - 1 ; i >= 0 && S[j+i] == T[i] ; i-- ) 
                    ;
                if ( i < 0 ) {
                    printres(j) ;
                    j += BmGs[0] ;  
                    flag = 1 ;  
                } 
                else {
                    j += max(BmGs[i],BmGc[S[i+j]]-tlen+1+i) ; 
                } 
            }
            if ( flag == 0 ) 
                printf("子串在主串中不匹配!\n") ; 
        }

        void printres(int index) {
            printf("子串在主串下标 %d 处匹配!\n",index) ; 
        }
} ; 


int main() {
    BM example ; 
    example.bm() ; 
    return 0 ; 
}