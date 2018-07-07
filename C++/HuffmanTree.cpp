#include<iostream>
#include<cstdio>
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
    struct HfNode **tree ;                // 用数字储存树的节点
    int root ;
} ;

int num = 10 ;
int keys[10] = {1,2,3,4,5,6,7,8,9,10} ;  // 一组权值

void Select (struct HuffmanTree &HT, int e, int &s1, int &s2 ) {
    s1 = -1 ; s2 = -1 ;
    for ( int i = 0 ; i <= e ; i++ ) {
        if ( HT.tree[i]->visit )
            continue ;
        if ( s1 == -1 ) {
            s1 = i ;
        }
        else if ( s2 == -1 ) {
            s2 = i ;
        }
        else if ( HT.tree[i]->val < max(HT.tree[s1]->val,HT.tree[s2]->val) ) {
            if ( HT.tree[s1]->val > HT.tree[s2]->val )
                s1 = i ;
            else
                s2 = i ;
        }
    }
    if ( s1 != -1)
        HT.tree[s1]->visit = true ;
    if ( s2 != -1 )
        HT.tree[s2]->visit = true ;
}

void CodingHuffmanTree(struct HuffmanTree &HT, int index_, int pos) {
    static int code[20] = {0} ;                    // 静态数组存储每个叶子节点的编码值
    if ( index_ > HT.root || index_ == -1 )
        return ;
    struct HfNode *Hn = HT.tree[index_] ;
    if ( Hn->left == -1 && Hn->right == -1 ) {         // 是叶子节点，输出哈夫曼编码
        printf("节点%c (权值为%d) 的点的哈夫曼编码为: ",Hn->val+'a'-1,Hn->val ) ;
        for ( int i = 0 ; i < pos ; i++ ) {
            printf("%d ",code[i]) ;
        }
        printf("\n") ;
    }
    else {
        code[pos] = 0 ;
        CodingHuffmanTree(HT,Hn->left,pos+1) ;
        code[pos] = 1 ;
        CodingHuffmanTree(HT,Hn->right,pos+1) ;
    }
}

void CreateHuffmanTree(struct HuffmanTree &HT, int *w, int n) {
    if ( n <= 1 )
        return ;
    int i, m = 2*n  ;                   // 整个树的总节点数为 2*n-1 个, 取  2*n个节点，最后一个节点是哨兵
    HT.tree = new HfNode *[m] ;
    struct HfNode **p = HT.tree ;

    for ( i = 0 ; i < n ;  i++ ){
        p[i] = new HfNode (w[i],-1,-1,false) ;
    }
    for ( ; i < m ; i++ ) {
        p[i] = new HfNode (0,-1,-1,false) ;
    }
    for ( i = n ; i < m - 1 ; i++ ) {
        int s1, s2 ;
        Select(HT,i-1,s1,s2) ;              // 找出权值最小的两个节点的下标
        HT.tree[i]->left = s1 ;
        HT.tree[i]->right = s2 ;
        if ( s1 != -1 && s2 != -1 )
            HT.tree[i]->val = HT.tree[s1]->val + HT.tree[s2]->val ;
        HT.tree[i]->visit = false ;
    }
    HT.root = m - 2 ;
    CodingHuffmanTree(HT,HT.root,0) ;
}


int main() {
    struct HuffmanTree HT ;
    CreateHuffmanTree(HT,keys,num) ;
    return 0 ;
}
