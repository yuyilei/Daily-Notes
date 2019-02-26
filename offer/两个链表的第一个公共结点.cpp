/*
输入两个链表，找出它们的第一个公共结点。
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
    ListNode* FindFirstCommonNode( ListNode* pHead1, ListNode* pHead2) {
        if ( !pHead1 || !pHead2 )
            return NULL;
        int len1 = 0, len2 = 0;
        for ( ListNode* head = pHead1 ; head ; head = head->next )
            len1++;
        for ( ListNode* head = pHead2 ; head ; head = head->next )
            len2++;
        while ( len1 > len2 ) {
            pHead1 = pHead1->next;
            len1--;
        }
        while ( len1 < len2 ) {
            pHead2 = pHead2->next;
            len2--;
        }
        while ( len1 > 0 ) {
            if ( pHead1 == pHead2 )
                return pHead1;
            pHead1 = pHead1->next;
            pHead2 = pHead2->next;
            len1--;
        }
        return NULL;
    }
};
