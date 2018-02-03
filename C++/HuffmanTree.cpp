#include<iostream>
using namespace std ;

// 构建赫夫曼树 
// 给定n个权值，初始化为n个子树  
// 选出权值最小的两个树，作为新树的左右子树，产生新的权值，在序列中删除这两个子树，加入新的树 
// 一直循环，直到只剩下一棵树 

struct HfNode {
    int val, left, right ; 
    bool visit ; 
    HfNode(int a, int b, int c, bool d) : val(a), left(b), right(c), visit(d) {}
} ; 

struct HuffmanTree {
    struct HfNode *tree ;                // 用数字储存树的节点 
    int root ; 
} ; 

int num = 10 ; 
int keys[10] = {1,2,3,4,5,6,7,8,9,10} ;  // 一组权值 

void Select (struct HuffmanTree &HT, int e, int &s1 , int &s2 ) {
    if ( e < 2 ) 
        return ; 
    for ( int i = 0 ; i <= e ; i++ ) {
        if ( HT.tree[i].visit ) 
            continue ;     
        HT.tree[i].visit = true ; 
        if ( HT.tree[i].val < max(HT.tree[s1].val,HT.tree[s2].val) ) { 
            if ( HT.tree[s1].val > HT.tree[s2].val ) 
                s1 = i ;
            else 
                s2 = i ; 
        }
    }
}

void CreateHuffmanTree(struct HuffmanTree &HT, int *w, int n) {
    if ( n <= 1 ) 
        return ; 
    int i, m = 2*n  ;                   // 整个树的总节点数为 2*n-1 个, 取  2*n个节点，最后一个节点是哨兵 
    struct HfNode *p = HT.tree ; 

    for ( i = 0 ; i < n ;  p++ )  
        p = new HfNode (w[i],-1,-1,false) ;  
    for ( ; i < m ; i++ ) 
        p = new HfNode (0,-1,-1,false) ; 

    for ( i = n ; i < m - 1 ; i++ ) {
        int s1, s2 ; 
        s1 = s2 = m-1 ; 
        Select(HT,i-1,s1,s2) ;              // 找出权值最小的两个节点的下标   
        HT.tree[i].left = s1 ; 
        HT.tree[i].right = s2 ;
        HT.tree[i].val = HT.tree[s1].val + HT.tree[s2].val ; 
    }
    HT.root = m - 2 ; 
}

int main() {
    struct HuffmanTree HT ; 
    CreateHuffmanTree(HT,keys,num) ; 
    return 0 ; 
}