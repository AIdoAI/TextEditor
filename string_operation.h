#ifndef   STRING_OPERATION       //如果没有定义这个宏
#define   STRING_OPERATION       //定义这个宏

#include <iostream>
#include <string.h>
#include "mainwindow.h"

using namespace std;

//注：所有字符串从1开始存储

class StrOperation{
    friend class MainWindow;

    public:

    private:
        /* 查找字符串(使用KMP算法) */
        bool FindString(Link headPtr, string str, struct Position &position);
        /* 替换字符串 */
        void ReplaceString(Link headPtr, string str, string replaceStr, struct Position &position);
        /* 查找or替换字符串的用户交互 (fr: "查找"or"替换")*/
        void StringInteraction(struct Position &position, Link headPtr, string fr, Text *text, Cursor *cursor);
};

#endif
