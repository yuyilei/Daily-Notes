/*
请实现一个函数，用来判断一颗二叉树是不是对称的。注意，如果一个二叉树同此二叉树的镜像是同样的，定义其为对称的。
*/

/*
struct TreeNode {
    int val;
    struct TreeNode *left;
    struct TreeNode *right;
    TreeNode(int x) :
            val(x), left(NULL), right(NULL) {
    }
};
*/
class Solution {
public:
    bool isSymmetrical(TreeNode* pRoot) {
        if ( !pRoot )
            return true;
        // 这个函数是比较两个二叉树的根结点的值，以及其对称位置的子树的根节点是否指相同 
        return isSame(pRoot->left,pRoot->right);
    }
    bool isSame(TreeNode* t1, TreeNode* t2) {
        if ( !t1 && !t2 )
            return true;
        if ( !t1 || !t2 )
            return false;
        return ( t1->val == t2->val && isSame(t1->left,t2->right) && isSame(t1->right,t2->left));
    }
};

