/*
输入两个单调递增的链表，输出两个链表合成后的链表，当然我们需要合成后的链表满足单调不减规则。
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
    ListNode* Merge(ListNode* pHead1, ListNode* pHead2) {
        ListNode* res = new ListNode(0);
        ListNode* head = res;
        while ( pHead1 && pHead2 ) {
            if ( pHead1->val <= pHead2->val ) {
                head->next = pHead1;
                pHead1 = pHead1->next;
            }
            else {
                head->next = pHead2;
                pHead2 = pHead2->next;
            }
            head = head->next;
        }
        if ( pHead1 )
            head->next = pHead1;
        if ( pHead2 )
            head->next = pHead2;
        return res->next;
    }
};
