/*
给一个链表，若其中包含环，请找出该链表的环的入口结点，否则，输出null。
*/

/*
struct ListNode {
    int val;
    struct ListNode *next;
    ListNode(int x) :
        val(x), next(NULL) {
    }
};
*/
class Solution {
public:
    ListNode* EntryNodeOfLoop(ListNode* pHead) {
        if ( pHead == NULL )
            return NULL;
        ListNode *fast = pHead, *slow = pHead;
        while ( true ) {
            // 先判断有没有环
            if ( !fast->next || !fast->next->next )
                return NULL;
            fast = fast->next->next;
            slow = slow->next;
            if ( fast == slow )
                break;
        }
        ListNode *res = pHead;
        while ( res != fast ) {
            res = res->next;
            fast = fast->next;
        }
        return res;
    }
};
