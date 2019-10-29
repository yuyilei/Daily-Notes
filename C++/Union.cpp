#include <iostream>
#include <cstdio>

using namespace std;

void reverse(string &s, int start, int end) {
    if (start >= end) 
        return;
    while (start < end) {
        char tmp = s[start];
        s[start] = s[end];
        s[end] = tmp;
        start++;
        end--;
    }
}

int main(){
    string res;
    cin >> res;
    if (res.size() <=1 ) {
        cout << res << endl;
        return 0;
    }
    int len = res.size()-1;
    reverse(res, 0, len-1);
    int start = 0;
    for (int i = 1; i <= len; i++) {
        if (i == len || res[i] == ' ') {
            reverse(res, start, i-1);
            while (i < len && res[i] == ' ' ) {
                i++;
                start = i;
            }
        }
    }
    cout << res << endl;
    return 0;
}