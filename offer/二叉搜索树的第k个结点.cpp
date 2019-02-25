/*
给定一棵二叉搜索树，请找出其中的第k小的结点。例如， （5，3，7，2，4，6，8）    中，按结点数值大小顺序第三小结点的值为4。
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
    TreeNode* KthNode(TreeNode* pRoot, int k) {
        if ( k == 0 )
            return NULL;
        inorderTree(pRoot);
        if ( inorder.size() >= k )
            return inorder[k-1];
        return NULL;
    }
    void inorderTree(TreeNode* pRoot) {
        if ( pRoot == NULL )
            return;
        inorderTree(pRoot->left);
        inorder.push_back(pRoot);
        inorderTree(pRoot->right);
    }
private:
    vector<TreeNode* > inorder;
};

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
    TreeNode* KthNode(TreeNode* pRoot, int k) {
        if ( !pRoot )
            return NULL;
        int i = 0;
        stack<TreeNode*> s;
        TreeNode* now = pRoot;
        while ( now || !s.empty() ) {
            while ( now ) {
                s.push(now);
                now = now->left;
            }
            i++;
            now = s.top();
            s.pop();
            if ( i == k )
                return now;
            now = now->right;
        }
        return NULL;
    }


};
