/*
输入一个整数数组，判断该数组是不是某二叉搜索树的后序遍历的结果。如果是则输出Yes,否则输出No。假设输入的数组的任意两个数字都互不相同。
*/

class Solution {
public:
    bool VerifySquenceOfBST(vector<int> sequence) {
        if ( sequence.empty() )
            return false;
        return verifyBST(sequence,0,sequence.size()-1);
    }
    bool verifyBST(vector<int>tree, int s, int e) {
        if ( s >= e )
            return true;
        int root = tree[e];
        int index = e - 1;
        while ( index >= s && tree[index] > root ) {
            index--;
        }
        for ( int i = index ; i >= s ; i-- ) {
            if ( tree[i] > root )
                return false;
        }
        bool res = true;
        res &= verifyBST(tree,s,index);
        res &= verifyBST(tree,index+1,e-1);
        return res;
    }
};
