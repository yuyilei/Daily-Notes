/*
给定一个二叉树和其中的一个结点，请找出中序遍历顺序的下一个结点并且返回。注意，树中的结点不仅包含左右子结点，同时包含指向父结点的指针。
*/

/*
struct TreeLinkNode {
    int val;
    struct TreeLinkNode *left;
    struct TreeLinkNode *right;
    struct TreeLinkNode *next;
    TreeLinkNode(int x) :val(x), left(NULL), right(NULL), next(NULL) {

    }
};
*/
/*
 1. 如果该节点有右子树，则返回右子树的最左节点
 2. 否则，如果该节点是父节点的左孩子，则返回父节点，否则，另该节点等于父节点，继续寻找 
 */
class Solution {
public:
    TreeLinkNode* GetNext(TreeLinkNode* pNode){
        if ( !pNode )
            return NULL;
        if ( pNode->right ) {
            TreeLinkNode* res = pNode->right;
            while ( res->left ) {
                res = res->left;
            }
            return res;
        }
        else {
            while ( pNode->next ) {
                TreeLinkNode* parent = pNode->next;
                if ( pNode == parent->left ) {
                    return parent;
                }
                pNode = pNode->next;
            }
            return NULL;
        }
    }
};
