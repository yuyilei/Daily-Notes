/*
输入一棵二叉树，判断该二叉树是否是平衡二叉树。
*/

// 从上到下，每个节点会被遍历多次，有不必要的开销 
class Solution {
public:
    bool IsBalanced_Solution(TreeNode* pRoot) {
        if ( !pRoot )
            return true;
        int lDepth = TreeDepth(pRoot->left);
        int rDepth = TreeDepth(pRoot->right);
        if ( abs(lDepth-rDepth) > 1 )
            return false;
        return IsBalanced_Solution(pRoot->left) & IsBalanced_Solution(pRoot->right);
    }
    int TreeDepth(TreeNode* pRoot) {
         if ( pRoot == NULL )
             return 0;
         return 1 + max(TreeDepth(pRoot->left),TreeDepth(pRoot->right));
     }
};


// 从下往上，先判断此树的左右子树是否为平衡二叉树，如果不是直接返回-1，如果左右子树都是平衡二叉树，就返回此树的高度 
// -1 表示不是平衡二叉树 
class Solution {
public:
    bool IsBalanced_Solution(TreeNode* pRoot) {
        return getDepth(pRoot) != -1;
    }
    int getDepth(TreeNode* root) {
        if ( !root ) return 0;
        int left = getDepth(root->left);
        if ( left == -1 ) return -1;
        int right = getDepth(root->right);
        if ( right == -1 ) return -1;
        return abs(left-right) > 1 ? -1 : max(left,right) + 1;
    }
};
