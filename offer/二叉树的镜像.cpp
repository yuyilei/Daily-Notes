/*
题目描述
操作给定的二叉树，将其变换为源二叉树的镜像。
输入描述:
二叉树的镜像定义：源二叉树 
    	    8
    	   /  \
    	  6   10
    	 / \  / \
    	5  7 9 11
    	镜像二叉树
    	    8
    	   /  \
    	  10   6
    	 / \  / \
    	11 9 7  5
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
class Solution {
public:
    void Mirror(TreeNode *pRoot) {
        if ( !pRoot )
            return;
        TreeNode *left = pRoot->left;
        TreeNode *right = pRoot->right;
        Mirror(left);
        Mirror(right);
        pRoot->right = left;
        pRoot->left = right;
    }
};


class Solution {
public:
    void Mirror(TreeNode *pRoot) {
        if ( !pRoot )
            return;
        stack<TreeNode*> s;
        s.push(pRoot);
        while ( !s.empty() ) {
            TreeNode *root = s.top();
            s.pop();
            TreeNode *tmp = root->left;
            root->left = root->right;
            root->right = tmp;
            if ( root->left )
                s.push(root->left);
            if ( root->right )
                s.push(root->right);
        }
    }
};
