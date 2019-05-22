/*
请实现一个函数用来判断字符串是否表示数值（包括整数和小数）。例如，字符串"+100","5e2","-123","3.1416"和"-1E-16"都表示数值。 但是"12e","1a3.14","1.2.3","+-5"和"12e+4.3"都不是。
 */

class Solution {
public:
    bool isNumeric(char* string) {
        if ( string == NULL || *string == '\0' )
            return false;
        if ( *string == '+' || *string == '-' )    // 跳过 +、-
            string++;
        bool bot = false, num = false, e = false;  // 分别表示是否出现过点、数字、e 
        while ( *string != '\0' ) {
            if ( *string >= '0' && *string <= '9' ) {
                num = true;
                string++;
            }
            else if ( *string == '.' ) {
                if ( bot || e )                    // 点的前面不能出现过 点、e 
                    return false;
                bot = true;
                *string++;
                if ( *string == '\0' )
                    return false;
            }
            else if ( *string == 'e' || *string == 'E' ) {
                if ( !num || e )                  // e 的前面一定要出现过数字，一定不能出现过 e 
                    return false;
                e = true;
                string++;
                if ( *string == '+' || *string == '-' ) // 跳过 +、-
                    string++;
                if ( *string == '\0' )
                    return false;
            }
            else {                                
                return false;
            }
        }
        return true;
    }

};
