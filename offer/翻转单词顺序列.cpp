/*
牛客最近来了一个新员工Fish，每天早晨总是会拿着一本英文杂志，写些句子在本子上。同事Cat对Fish写的内容颇感兴趣，有一天他向Fish借来翻看，但却读不懂它的意思。例如，“student. a am I”。后来才意识到，这家伙原来把句子单词的顺序翻转了，正确的句子应该是“I am a student.”。Cat对一一的翻转这些单词顺序可不在行，你能帮助他么？
*/

class Solution {
public:
    string ReverseSentence(string str) {
        reverse(str, 0, str.size()-1);
        int last = 0;
        for ( int i = 1 ; i < str.size() ; i++ ) {
            if ( str[i] == ' ' ) {
                reverse(str, last, i-1);
                last = i+1;
            }
        }
        // 翻转最后一个单词
        reverse(str, last, str.size()-1);
        return str;
    }
    void reverse(string &str, int s, int e ) {
        while ( s < e ) {
            swap(str[s], str[e]);
            s++; e--;
        }
    }
};
