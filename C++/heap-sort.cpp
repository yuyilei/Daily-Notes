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

void heap_adjust(Item a[], int s, int e) {
    int parent = s , child = 2*s+1 , tmp = a[s] ;
    while ( child <= e ){
        if ( child < e && less(a[child],a[child+1])) child++ ;
        if ( tmp >= a[child] )
            break ;
        a[parent] = a[child] ;
        parent = child ;
        child = child*2 + 1 ;
}
    a[parent] = tmp ;
}

void heap_sort(Item a[], int n){
    for (int i = n/2-1 ; i >=0 ; i-- )
        heap_adjust(a,i,n-1) ;
    for ( int i = n-1 ; i > 0 ; i-- ){
        exch(a[0],a[i]) ;
        heap_adjust(a,0,i-1) ;
    }
}


int main(){
    Item tmp[] = {5,8,13,2,31,0,6,19,9,1,18,3,7,4,14} ;
    cout << "Before heap sort:" << endl ;
    show_elem(tmp,15) ;
    heap_sort(tmp,15) ;
    cout << "After heap sort:" << endl ;
    show_elem(tmp,15) ;
    return 0 ;
}