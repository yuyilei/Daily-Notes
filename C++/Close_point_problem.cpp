#include<cstdio>
#include<iostream>
#include<vector>
#include<algorithm> 
#include<cmath> 
using namespace std ; 

/*
分治法:求最近点问题
测试样例：
    输入点集中点的个数：
    5
    输入各个点的x,y坐标：
    1 3
    -2 1
    -4 3
    0 0
    5 1
    最近点距离为2.236

对于点集s:

double closepoint(S){
    if ( S.size() == 1 ) 
        return MAXD ;
    mid = mid(S)        // S的中点(以x坐标)
    S1 = S中x坐标小于m
    S2 = S中x坐标大于m
    d1 = closepoint(S1)
    d2 = closepoint(S2) 
    d = min(d1,d2) 
    P1 = S1中x坐标与m小于d的点集
    P2 = S2中x坐标与m小于d的点集
    将P1和P2中的点根据y坐标排序 
    for ( q1: P1 ) {
        for ( q2 : P2 ) {
            tmpd = dis(q1,q2) 
            d = min(tmpd,d) 
        }
    }
    return d 
}

时间复杂度: O(n*log(n)) 
*/

const double MAXD = 999999 ; 
struct point {
    int x, y ; 
} ; 

bool cmpxy(struct point a, struct point b) {
    if ( a.x != b.x )   
        return a.x < b.x ; 
    return a.y < b.y ; 
}

bool cmpy(struct point a, struct point b) {
    return a.y < b.y ; 
}

class ClosePoint {
    private : 
        int n ;
        double d ; 
        struct point * s ; 
    public :
        ClosePoint() {
            d = MAXD ; 
            cout << "输入点集中点的个数：" << endl ; 
            cin >> n ; 
            s = new struct point [n] ; 
            cout << "输入各个点的x,y坐标：" << endl ; 
            for ( int i = 0 ; i < n ; i++ ) {
                cin >> s[i].x >> s[i].y ; 
            }
        }
        void closepoint() {
            sort(s,s+n,cmpxy) ; 
            d = segmentclose(0,n-1) ; 
        }
        double dis(struct point a, struct point b) {
            return sqrt((a.x-b.x)*(a.x-b.x) + (a.y-b.y)*(a.y-b.y)) ;  
        }
        void showd() {
            printf("最近点距离为%.3lf\n",d) ; 
        }
        double segmentclose(int left, int right) {
            if ( left == right ) 
                return MAXD ; 
            if ( left + 1 == right ) 
                return dis(s[left],s[right]) ; 
            if ( left + 2 == right ) 
                return min(dis(s[left],s[left+1]),dis(s[left+1],s[right])) ; 
            int mid = ( left + right ) / 2 ; 
            double tmpd = min(segmentclose(left,mid),segmentclose(mid+1,right)) ;  
            vector<struct point> tid ; 
            for ( int i = left ; i <= right ; i++) {
                if ( fabs(s[i].x - s[mid].x) <= tmpd ) 
                    tid.push_back(s[i]) ;  
            } 
            sort(tid.begin(),tid.end(),cmpy) ; 
            for ( int i = 0 ; i < tid.size() ; i++ ) {
                for ( int j = i + 1 ; j < tid.size() ; j++ ) {
                    if ( fabs(tid[i].y-tid[j].y) >= tmpd ) 
                        break ; 
                    if ( dis(tid[i],tid[j]) < tmpd ) 
                        tmpd = dis(tid[i],tid[j]) ; 
                }
            }
            return tmpd ; 
        }
} ; 

int main() {
    ClosePoint P ; 
    P.closepoint() ; 
    P.showd() ; 
    return 0 ; 
}