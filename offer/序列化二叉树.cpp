/*
请实现两个函数，分别用来序列化和反序列化二叉树
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
    char* Serialize(TreeNode *root) {
        stringstream ss;
        serialize(root, ss);
        string str = ss.str();
        char* res = new char[str.length()+1];
        strcpy(res, str.c_str());
        return res;
    }
    void serialize(TreeNode *root, stringstream &ss) {
        if ( !root ) {
            ss << "#";
            return;
        }
        ss << to_string(root->val);
        serialize(root->left, ss);
        serialize(root->right, ss);
    }
    TreeNode* Deserialize(char *str) {
        stringstream ss;
        ss << str;
        TreeNode* root = deserialize(ss);
        return root;
    }
    TreeNode* deserialize(stringstream &ss) {
        string str;
        ss >> str;
        if ( str == "#" )
            return NULL;
        TreeNode* root = new TreeNode(atoi(str.c_str()));
        root->left = deserialize(ss);
        root->right = deserialize(ss);
        return root;
    }
};
