/*
 输入一个链表，反转链表后，输出新链表的表头。
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
    ListNode* ReverseList(ListNode* pHead) {
        ListNode* res = NULL;
        while ( pHead ) {
            ListNode* next = pHead->next;
            pHead->next = res;
            res = pHead;
            pHead = next;
        }
        return res;
    }
};
