/*
输入某二叉树的前序遍历和中序遍历的结果，请重建出该二叉树。假设输入的前序遍历和中序遍历的结果中都不含重复的数字。例如输入前序遍历序列{1,2,4,7,3,5,6,8}和中序遍历序列{4,7,2,1,5,3,8,6}，则重建二叉树并返回。
 */

// C++里面这种题还是用 index比较好 

/**
 * Definition for binary tree
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
 */
class Solution {
public:
    TreeNode* reConstructBinaryTree(vector<int> pre,vector<int> vin) {
        return buildTree(pre,0,pre.size()-1,vin,0,vin.size()-1);
    }
    TreeNode* buildTree(vector<int>pre, int ps, int pe, vector<int>vin, int vs, int ve) {
        if ( ps > pe ) 
            return NULL;
        TreeNode* res = new TreeNode(pre[ps]);
        int index = vs;
        while ( index <= ve && vin[index] != pre[ps] ) 
            index++;
        res->left = buildTree(pre,ps+1,ps+index-vs,vin,vs,index-1);
        res->right = buildTree(pre,ps+index-vs+1,pe,vin,index+1,ve);
        return res;
    }
};
