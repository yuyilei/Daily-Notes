/*
输入一棵二叉搜索树，将该二叉搜索树转换成一个排序的双向链表。要求不能创建任何新的结点，只能调整树中结点指针的指向。
 */
/*
struct TreeNode {
	int val;
	struct TreeNode *left;
	struct TreeNode *right;
	TreeNode(int x) :
			val(x), left(NULL), right(NULL) {
	}
};*/

/*
 非递归中序遍历
 now = root;
 stack<node> s;
 while ( now || !s.empty() ) {
    while ( now ) {
        s.push(now);
        now = now->left;
    }
    now = s.top();
    s.pop();
    // do something...
    now = now->right;
 }
 */

class Solution {
public:
    TreeNode* Convert(TreeNode* pRootOfTree) {
        if ( !pRootOfTree )
            return NULL;
        TreeNode *res = new TreeNode(0);
        TreeNode *head = res;
        TreeNode *now = pRootOfTree;
        stack<TreeNode*> s;
        while ( now || !s.empty() ) {
            while ( now ) {
                s.push(now);
                now = now->left;
            }
            now = s.top();
            s.pop();
            head->right = now;
            now->left = head;
            head = now;
            now = now->right;
        }
        res->right->left = NULL;    // 第一个节点指向res，需要手动清除
        return res->right;
    }
};
