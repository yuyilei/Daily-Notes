/*
从上到下按层打印二叉树，同一层结点从左至右输出。每一层输出一行。
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
        vector<vector<int> > Print(TreeNode* pRoot) {
            vector<vector<int>> res;
            if ( !pRoot )
                return res;
            queue<TreeNode*> q;
            q.push(pRoot);
            while ( !q.empty() ) {
                int len = q.size();
                vector<int> tmp;
                for ( int i = 0 ; i < len ; i++ ) {
                    TreeNode *now = q.front();
                    q.pop();
                    tmp.push_back(now->val);
                    if ( now->left ) q.push(now->left);
                    if ( now->right ) q.push(now->right);
                }
                res.push_back(tmp);
            }
            return res;
        }

};
