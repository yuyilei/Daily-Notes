/*
输入两棵二叉树A，B，判断B是不是A的子结构。（ps：我们约定空树不是任意一个树的子结构）
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
// 子结构不一定要到叶子节点都相同
class Solution {
public:
    bool HasSubtree(TreeNode* pRoot1, TreeNode* pRoot2) {
        if ( !pRoot1 || !pRoot2 )
            return false;
        return dfsHasSubtree(pRoot1,pRoot2) || HasSubtree(pRoot1->left,pRoot2) || HasSubtree(pRoot1->right,pRoot2);
    }
    bool dfsHasSubtree(TreeNode* t1, TreeNode* t2) {
        if ( !t2 )
            return true;
        if ( !t1 )
            return false;
        return (t1->val == t2->val) && dfsHasSubtree(t1->left,t2->left) && dfsHasSubtree(t1->right,t2->right);
    }
};
