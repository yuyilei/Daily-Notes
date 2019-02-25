/*
请实现一个函数按照之字形打印二叉树，即第一行按照从左到右的顺序打印，第二层按照从右至左的顺序打印，第三行按照从左到右的顺序打印，其他行以此类推。
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
        int flag = 0;
        queue<TreeNode*> q;
        q.push(pRoot);
        while ( !q.empty() ) {
            int len = q.size();
            vector<int> one;
            for ( int i = 0 ; i < len ; i++ ) {
                TreeNode* now = q.front();
                q.pop();
                one.push_back(now->val);
                if ( now->left ) 
                    q.push(now->left);
                if ( now->right ) 
                    q.push(now->right);
            }
            if ( flag % 2 ) 
                reverse(one.begin(),one.end());
            flag++;
            res.push_back(one);
        }
        return res;
    }
    
};
