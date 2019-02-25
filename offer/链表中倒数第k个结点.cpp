/*
输入一个链表，输出该链表中倒数第k个结点。
 */

/*
struct ListNode {
	int val;
	struct ListNode *next;
	ListNode(int x) :
			val(x), next(NULL) {
	}
};*/
class Solution {
public:
    ListNode* FindKthToTail(ListNode* pListHead, unsigned int k) {
        int len = 0;
        for ( ListNode* head = pListHead ; head ; head = head->next ) {
            len++;
        }
        if ( k > len )
            return NULL;
        ListNode* res = pListHead;
        for ( int i = 0 ; i < len - k ; i++ ) {
            res = res->next;
        }
        return res;
    }
};


// 只用遍历一遍的方法 
class Solution {
public:
    ListNode* FindKthToTail(ListNode* pListHead, unsigned int k) {
        ListNode *p = pListHead;
        ListNode *q = pListHead;
        int i = 0;
        for ( ; p ; p = p->next, i++ ) {
            if ( i >= k ) 
                q = q->next;
        }
        return ( i >= k ) ? q : NULL;
    }
}; 
