/*
输入一个复杂链表（每个节点中有节点值，以及两个指针，一个指向下一个节点，另一个特殊指针指向任意一个节点），返回结果为复制后复杂链表的head。（注意，输出结果中请不要返回参数中的节点引用，否则判题程序会直接返回空）
*/

/*
struct RandomListNode {
    int label;
    struct RandomListNode *next, *random;
    RandomListNode(int x) :
            label(x), next(NULL), random(NULL) {
    }
};
*/
class Solution {
public:
    RandomListNode* Clone(RandomListNode* pHead) {
        if ( !pHead )
            return NULL;
        vector<int> labels;                                           // 记录每个节点中的label
        map<RandomListNode*, int> index;                              // 记录每个节点的索引
        map<int, int> random;                                         // 记录每个节点的random的索引
        map<int, RandomListNode*> rnode;                              // 记录克隆得到的每个节点，索引对应节点
        RandomListNode* res = new RandomListNode(0);
        RandomListNode* now = res;
        int i = 0, j = 0, k = 0;
        for ( RandomListNode* p = pHead ; p ; p = p->next ) {
            labels.push_back(p->label);
            index[p] = i++;
        }
        RandomListNode* nullnode = NULL;
        index[nullnode] = i;                                          // 记录空节点的索引为最后
        for ( RandomListNode* p = pHead ; p ; p = p->next ) {
            random[index[p]] = index[p->random];
        }
        for ( ; j < labels.size() ; j++ ) {
            RandomListNode* tmp = new RandomListNode(labels[j]);
            now->next = tmp;
            now = now->next;
            rnode[j] = tmp;
        }
        for ( RandomListNode* t = res->next ; k < labels.size() ; k++ ) {
            if ( random[k] == i )                                     // 此节点的random为NULL
                t->random = NULL;
            else
                t->random = rnode[random[k]];
            t = t->next;
        }
        return res->next;
    }
};
