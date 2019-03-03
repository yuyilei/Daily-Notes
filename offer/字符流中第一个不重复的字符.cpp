/*
请实现一个函数用来找出字符流中第一个只出现一次的字符。例如，当从字符流中只读出前两个字符"go"时，第一个只出现一次的字符是"g"。当从该字符流中读出前六个字符“google"时，第一个只出现一次的字符是"l"。
如果当前字符流没有存在出现一次的字符，返回#字符。
*/

class Solution
{
public:
  //Insert one char from stringstream
    void Insert(char ch) {
        if ( counter.find(ch) != counter.end() )
            counter[ch]++;
        else {
            counter[ch] = 1;
            q.push(ch);
        }
    }
  //return the first appearence once char in current stringstream
    char FirstAppearingOnce() {
        while ( !q.empty() && counter[q.front()] != 1 )
           q.pop();
        if ( !q.empty() )
            return q.front();
        return '#';
    }
private:
    map<char, int> counter;
    queue<char> q;
};
