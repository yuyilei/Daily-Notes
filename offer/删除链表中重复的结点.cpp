/*
在一个排序的链表中，存在重复的结点，请删除该链表中重复的结点，重复的结点不保留，返回链表头指针。 例如，链表1->2->3->3->4->4->5 处理后为 1->2->5
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
    ListNode* deleteDuplication(ListNode* pHead) {
        if ( !pHead || !pHead->next )
            return pHead;
        ListNode* res = new ListNode(0);
        res->next = pHead;
        ListNode* pre = res;
        ListNode* now = res->next;
        while ( now ) {
            int flag = 0;
            while ( now->next && now->next->val == now->val ) {
                now = now->next;
                flag = 1;
            }
            // 有重复节点，通过 pre 删除这些重复节点 
            if ( flag == 1 ) {
                pre->next = now->next;
            }
            else {
                pre = pre->next;
            }
            now = now->next;
        }
        return res->next;
    }
};
