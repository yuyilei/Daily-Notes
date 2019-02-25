/*
输入一颗二叉树的跟节点和一个整数，打印出二叉树中结点值的和为输入整数的所有路径。路径定义为从树的根结点开始往下一直到叶结点所经过的结点形成一条路径。(注意: 在返回值的list中，数组长度大的数组靠前)
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
    vector<vector<int> > FindPath(TreeNode* root,int expectNumber) {
        vector<vector<int>> res;
        vector<int> one;
        if ( !root )
            return res;
        FindEachPath(root, expectNumber, res, one);
        return res;
    }
    // dfs
    void FindEachPath(TreeNode* root, int e, vector<vector<int>> &res, vector<int> &one) {
        one.push_back(root->val);
        if ( root->val == e && !root->left && !root->right ) {
            res.push_back(one);
        }
        if ( root->left )
            FindEachPath(root->left, e-root->val, res, one);
        if ( root->right )
            FindEachPath(root->right, e-root->val, res, one);
        one.pop_back();
    }
};
