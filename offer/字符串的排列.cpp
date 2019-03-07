/*
输入一个字符串,按字典序打印出该字符串中字符的所有排列。例如输入字符串abc,则打印出由字符a,b,c所能排列出来的所有字符串abc,acb,bac,bca,cab和cba。
输入描述:
输入一个字符串,长度不超过9(可能有字符重复),字符只包括大小写字母。
*/

class Solution {
public:
    vector<string> Permutation(string str) {
        vector<string> res;
        if ( str.empty() )
            return res;
        permutation(res, str, 0);
        sort(res.begin(), res.end());
        return res;
    }
private:
    void permutation(vector<string>&res, string str, int s) {
        if ( s == str.size() )
            res.push_back(str);
        else {
            for ( int i = s ; i < str.size() ; i++ ) {
                if ( i != s && str[i] == str[s] )
                    continue;
                swap(str[i], str[s]);
                // 注意这个地方是 s+1 不是 i+1
                permutation(res, str, s+1);
                swap(str[i], str[s]);
            }
        }
    }
};
