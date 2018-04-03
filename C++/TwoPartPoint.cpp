#include<cstdio>
#include<vector>
#include<algorithm>
using namespace std ;

/*
对每个点的横坐标进行排序，获取中点的横坐标，根据中点的横坐标划分为两个点数大致相同的子集 
*/

struct point {
    int x, y ; 
} ; 

int N = 15 ; 
int xy[15][2] = {{4,1},{0,1},{-2,-4},{-4,-1},{5,3},{10,3},{7,2},{-3,-5},{2,9},{0,0},{-1,1},{-10,2},{8,-2},{3,-5},{5,-3}} ; 
vector<struct point> v ; 

bool cmp ( struct point a, struct point b ) {
    return a.x > b.x ; 
}

void init() {
    for ( int i = 0 ; i < N ; i++ ) {
        struct point tmp ;   
        tmp.x = xy[i][0] ; 
        tmp.y = xy[i][1] ; 
        v.push_back(tmp) ; 
    } 
}

int main() {    
    init() ; 
    printf("集合S:\n") ; 
    for ( int i = 0 ; i < N ; i++ ) 
        printf("（%3d, %3d）\n",v[i].x,v[i].y) ; 
    printf("\n") ; 
    sort(v.begin(),v.end(),cmp) ; 
    printf("以 x = %d 为界，划分为两个子集\n",v[N/2].x) ; 
    printf("子集S1:\n") ; 
    for ( int i = 0 ; i <= N/2 ; i++ ) 
        printf("（%3d, %3d）\n",v[i].x,v[i].y) ; 
    printf("\n") ;
    printf("子集S2:\n") ; 
    for ( int i = N/2+1 ; i < N ; i++ ) 
        printf("（%3d, %3d）\n",v[i].x,v[i].y) ; 
    return 0 ; 
}