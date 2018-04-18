#include<cstdio>
#include<algorithm> 
using namespace std ; 

char x[] = {'a','b','c','b','d','b'} ; 
char y[] = {'a','c','b','b','a','b','d','b','b'} ; 

/*
求最长公共子序列,动态规划
设 二维数组help[lena+1][lenb+1]
定义：
help[i][j] = a[0,...,i-1]与b[0,...,j-1]的最长公共子序列的长度
动态规划方程：
1) help[i][j] = 0                                      i = 0 或 j = 0 
2) help[i][j] = help[i-1][j-1] +1                      i > 0 && j > 0 && a[i-1] = b[j-1]
3) help[i][j] = max(help[i-1][j],help[i][j-1])         i > 0 && j > 0 && a[i-1] != b[j-1]

*/

int Longest_Common_Subsequence(char *a, int lena, char *b, int lenb) {
    int **help = new int *[lena+1] ; 
    int **path = new int *[lena+1] ; 
    int i, j ; 
    for ( i = 0 ; i <= lena ; i++ ) { 
        help[i] = new int [lenb+1] ; 
        path[i] = new int [lenb+1] ;
    }
    for ( i = 0 ; i <= lena ; i++ ) {
        for ( j = 0 ; j <= lenb ; j++ ) {
            if ( i == 0 || j == 0 ) 
                help[i][j] = 0 ; 
            else if ( a[i-1] == b[j-1] ) {                     // a, b 序列的长度是从0开始的所以要减一  
                help[i][j] = help[i-1][j-1] + 1 ; 
                path[i][j] = 1 ; 
            } 
            else if ( help[i-1][j] > help[i][j-1] ){
                help[i][j] = help[i-1][j] ; 
                path[i][j] = 2 ; 
            }
            else {
                help[i][j] = help[i][j-1] ; 
                path[i][j] = 3 ; 
            }
        }
    }
    int index = help[lena][lenb] ; 
    char *res = new char[index+1] ; 
    i = lena ; j = lenb ; 
    while ( i > 0 && j > 0 ) {
        if ( path[i][j] == 1 ) {
            res[index--] = a[i-1] ; 
            i--; j--; 
        }
        else if ( path[i][j] == 2 ) 
            i-- ;
        else 
            j-- ; 
    } 
    printf("最长公共子序列：\n") ; 
    for ( i = 1 ; i <= help[lena][lenb] ; i++ ) {
        printf("%c ",res[i]) ; 
    }
    printf("\n") ; 
    return help[lena][lenb] ; 
}
int main() {
    int res = Longest_Common_Subsequence(x,6,y,9) ;  
    printf("longest: %d\n",res) ; 
    return 0 ; 
}