#include<iostream>
#include<cstdio>
#include<vector>
using namespace std ;
typedef int Item ;
#define key(A) (A)
#define less(A,B) (key(A) < key(B))
#define exch(A,B) { Item t = A ; A = B ; B = t ; }
#define compexch(A,B) if (less(B,A)) exch(A,B)

void show_elem(Item a[] , int num ) {
    for ( int i = 0 ; i < num ; i++ )
        cout << a[i] << " " ;
    cout << endl ;
}

void shell_sort(Item a[], int l){
    int h = 1 , i , j  ;
    while ( h < l/3 )
        h = h*3 + 1 ;
    while ( h >= 1 ){
        for ( i = h ; i < l ; i++ ){
            Item tmp = a[i] ;
            for ( j = i ; j >= h && less(tmp,a[j-h]) ; j -= h )
                a[j] = a[j-h] ;
            a[j] = tmp ;
        }
        h /= 3 ;
    }
}


int main(){
    Item tmp[] = {5,8,13,2,31,0,6,19,9,1,18,3,7,4,14} ;
    cout << "Before shell sort:" << endl ;
    show_elem(tmp,15) ;
    shell_sort(tmp,15) ;
    cout << "After shell sort:" << endl ;
    show_elem(tmp,15) ;
    return 0 ;
}
